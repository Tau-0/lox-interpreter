#pragma once

#include <data_structures/ast/expressions.hpp>
#include <data_structures/ast/value.hpp>

namespace lox {

class AstInterpreter {
 public:
    Value Interpret(const expressions::Expr& expr) {
        try {
            return Evaluate(expr);
        } catch (...) {
        }
    }

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
            Value rhs = Evaluate(*arg.expr_);
            if (arg.op_.GetType() == tokens::Type::kMinus) {
                return Value(-rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kBang) {
                return Value(IsTruthy(rhs));
            }
            return rhs;
        } else if constexpr (std::is_same_v<Arg, expressions::Binary>) {
            Value lhs = Evaluate(*arg.left_);
            Value rhs = Evaluate(*arg.right_);
            if (arg.op_.GetType() == tokens::Type::kMinus) {
                return Value(lhs.As<double>() - rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kStar) {
                return Value(lhs.As<double>() * rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kSlash) {
                return Value(lhs.As<double>() / rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kPlus) {
                if (lhs.Is<std::string>() && rhs.Is<std::string>()) {
                    return Value(lhs.As<std::string>() + rhs.As<std::string>());
                } else if (lhs.Is<double>() && rhs.Is<double>()) {
                    return Value(lhs.As<double>() + rhs.As<double>());
                }
            } else if (arg.op_.GetType() == tokens::Type::kGreater) {
                return Value(lhs.As<double>() > rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kGreaterEqual) {
                return Value(lhs.As<double>() >= rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kLessEqual) {
                return Value(lhs.As<double>() <= rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kLess) {
                return Value(lhs.As<double>() < rhs.As<double>());
            } else if (arg.op_.GetType() == tokens::Type::kBangEqual) {
                return Value(lhs != rhs);
            } else if (arg.op_.GetType() == tokens::Type::kEqualEqual) {
                return Value(lhs == rhs);
            }

            return {};

        } else if constexpr (std::is_same_v<Arg, expressions::Conditional>) {
            return {};
        } else if constexpr (std::is_same_v<Arg, expressions::Grouping>) {
            return {};
        } else {
            throw std::runtime_error("Unexpected expression type.");
        }
    }

 private:
    Value Evaluate(const expressions::Expr& expr) const;
    bool IsTruthy(const Value& value) const;
};

}  // namespace lox
