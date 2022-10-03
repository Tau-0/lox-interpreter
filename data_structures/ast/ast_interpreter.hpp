#pragma once

#include <data_structures/ast/expressions.hpp>
#include <data_structures/ast/value.hpp>

namespace lox {

class AstInterpreter {
 public:
    Value Interpret(const expressions::Expr& expr) const;

    template <typename Arg>
    Value operator()(const Arg& arg) const {
        if constexpr (std::is_same_v<Arg, expressions::String>) {
            return Value(arg.value_);
        } else if constexpr (std::is_same_v<Arg, expressions::Number>) {
            return Value(arg.value_);
        } else if constexpr (std::is_same_v<Arg, expressions::Boolean>) {
            return Value(arg.value_);
        } else if constexpr (std::is_same_v<Arg, expressions::Nil>) {
            return {};
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

 private:
    Value Evaluate(const expressions::Expr& expr) const;
    Value EvaluateUnary(const expressions::Unary& expr) const;
    Value EvaluateBinary(const expressions::Binary& expr) const;
    Value EvaluateConditional(const expressions::Conditional& expr) const;
    bool IsTruthy(const Value& value) const;
};

}  // namespace lox
