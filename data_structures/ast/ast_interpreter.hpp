#pragma once

#include <data_structures/ast/expressions.hpp>
#include <data_structures/ast/value.hpp>

namespace lox {

template <typename T>
concept IsLiteral = std::is_same_v<T, expressions::String> || std::is_same_v<T, expressions::Number> ||
                    std::is_same_v<T, expressions::Boolean> || std::is_same_v<T, expressions::Nil>;

class AstInterpreter {
 public:
    Value Interpret(const expressions::Expr& expr) const;

    template <typename Arg>
    Value operator()(const Arg& arg) const {
        if constexpr (IsLiteral<Arg>) {
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

 private:
    Value Evaluate(const expressions::Expr& expr) const;
    Value EvaluateUnary(const expressions::Unary& expr) const;
    Value EvaluateBinary(const expressions::Binary& expr) const;
    Value EvaluateConditional(const expressions::Conditional& expr) const;
    bool IsTruthy(const Value& value) const;
};

}  // namespace lox
