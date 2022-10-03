#pragma once

#include <data_structures/ast/expressions.hpp>
#include <data_structures/ast/statements.hpp>
#include <data_structures/ast/value.hpp>

namespace lox {

class Lox;

class AstInterpreter {
 public:
    explicit AstInterpreter(Lox& lox);
    Value Interpret(const expressions::Expr& expr) const;

    template <expressions::IsExpression Arg>
    Value operator()(const Arg& arg) const {
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
        } else {
            throw std::runtime_error("Unexpected expression type.");
        }
    }

    template <statements::IsStatement Arg>
    void operator()(const Arg& arg) const {

    }

    private : Value Evaluate(const expressions::Expr& expr) const;
    Value EvaluateUnary(const expressions::Unary& expr) const;
    Value EvaluateBinary(const expressions::Binary& expr) const;
    Value EvaluateConditional(const expressions::Conditional& expr) const;
    Value NumberOperation(const tokens::Token& op, double lhs, double rhs) const;
    Value SumOrConcatenate(const tokens::Token& op, const Value& lhs, const Value& rhs) const;
    bool IsTruthy(const Value& value) const;
    void CheckNumberOperand(const tokens::Token& op, const Value& value) const;
    void CheckNumberOperands(const tokens::Token& op, const lox::Value& lhs, const lox::Value& rhs) const;

 private:
    Lox& lox_;
};

}  // namespace lox
