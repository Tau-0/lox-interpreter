#pragma once

#include <data_structures/ast/expressions.hpp>
#include <data_structures/ast/statements.hpp>
#include <data_structures/ast/value.hpp>
#include <data_structures/environment/environment.hpp>
#include <iostream>
#include <vector>

namespace lox {

class Lox;

class AstInterpreter {
 public:
    explicit AstInterpreter(Lox& lox);
    void Interpret(const std::vector<statements::Stmt>& statements);

    template <expressions::IsExpression Arg>
    Value operator()(const Arg& arg) {
        if constexpr (expressions::IsLiteral<Arg>) {
            return Value(arg.value_);
        } else if constexpr (std::is_same_v<Arg, expressions::Unary>) {
            return EvaluateUnary(arg);
        } else if constexpr (std::is_same_v<Arg, expressions::Binary>) {
            return EvaluateBinary(arg);
        } else if constexpr (std::is_same_v<Arg, expressions::Conditional>) {
            return EvaluateConditional(arg);
        } else if constexpr (std::is_same_v<Arg, expressions::Grouping>) {
            return Evaluate(*arg.expr_);
        } else if constexpr (std::is_same_v<Arg, expressions::Variable>) {
            return environment_.Get(arg.name_);
        } else if constexpr (std::is_same_v<Arg, expressions::Assign>) {
            auto value = Evaluate(*arg.value_);
            environment_.Assign(arg.name_, value);
            return value;
        } else if constexpr (std::is_same_v<Arg, expressions::Logical>) {
            Value lhs = Evaluate(*arg.left_);
            if (arg.op_.GetType() == tokens::Type::kOr) {
                if (IsTruthy(lhs)) {
                    return lhs;
                }
            } else if (!IsTruthy(lhs)) {
                return lhs;
            }
            return Evaluate(*arg.right_);
        } else {
            throw std::runtime_error("Unexpected expression type.");
        }
    }

    template <statements::IsStatement Arg>
    void operator()(const Arg& arg) {
        if constexpr (std::is_same_v<Arg, statements::Print>) {
            auto value = Evaluate(*arg.expr_);
            std::cout << value.Stringify() << "\n";
        } else if constexpr (std::is_same_v<Arg, statements::Expression>) {
            Evaluate(*arg.expr_);
        } else if constexpr (std::is_same_v<Arg, statements::Var>) {
            Value value;
            if (arg.initializer_ != nullptr) {
                value = Evaluate(*arg.initializer_);
            }
            environment_.Define(arg.name_.GetLexeme(), value);
        } else if constexpr (std::is_same_v<Arg, statements::Block>) {
            ExecuteBlock(arg);
        } else if constexpr (std::is_same_v<Arg, statements::If>) {
            if (IsTruthy(Evaluate(*arg.condition_))) {
                Execute(*arg.then_branch_);
            } else if (arg.else_branch_ != nullptr) {
                Execute(*arg.else_branch_);
            }
        } else if constexpr (std::is_same_v<Arg, statements::While>) {
            while (IsTruthy(Evaluate(*arg.condition_))) {
                Execute(*arg.statement_);
            }
        } else {
            throw std::runtime_error("Unexpected statement type.");
        }
    }

 private:
    void Execute(const statements::Stmt& stmt);
    void ExecuteBlock(const statements::Block& block);
    Value Evaluate(const expressions::Expr& expr);
    Value EvaluateUnary(const expressions::Unary& expr);
    Value EvaluateBinary(const expressions::Binary& expr);
    Value EvaluateConditional(const expressions::Conditional& expr);
    Value NumberOperation(const tokens::Token& op, double lhs, double rhs) const;
    Value SumOrConcatenate(const tokens::Token& op, const Value& lhs, const Value& rhs) const;
    bool IsTruthy(const Value& value) const;
    void CheckNumberOperand(const tokens::Token& op, const Value& value) const;
    void CheckNumberOperands(const tokens::Token& op, const lox::Value& lhs, const lox::Value& rhs) const;

 private:
    Environment environment_;
    Lox& lox_;
};

}  // namespace lox
