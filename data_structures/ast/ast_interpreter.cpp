#include "ast_interpreter.hpp"

namespace lox {

Value AstInterpreter::Interpret(const expressions::Expr& expr) const {
    try {
        return Evaluate(expr);
    } catch (...) {
    }
    return {};
}

Value AstInterpreter::Evaluate(const expressions::Expr& expr) const {
    return expr.Accept(*this);
}

Value AstInterpreter::EvaluateUnary(const expressions::Unary& expr) const {
    Value rhs = Evaluate(*expr.expr_);
    if (expr.op_.GetType() == tokens::Type::kMinus) {
        return Value(-rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kBang) {
        return Value(IsTruthy(rhs));
    }
    return rhs;
}

Value AstInterpreter::EvaluateBinary(const expressions::Binary& expr) const {
    Value lhs = Evaluate(*expr.left_);
    Value rhs = Evaluate(*expr.right_);
    if (expr.op_.GetType() == tokens::Type::kMinus) {
        return Value(lhs.As<double>() - rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kStar) {
        return Value(lhs.As<double>() * rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kSlash) {
        return Value(lhs.As<double>() / rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kPlus) {
        if (lhs.Is<std::string>() && rhs.Is<std::string>()) {
            return Value(lhs.As<std::string>() + rhs.As<std::string>());
        } else if (lhs.Is<double>() && rhs.Is<double>()) {
            return Value(lhs.As<double>() + rhs.As<double>());
        }
    } else if (expr.op_.GetType() == tokens::Type::kGreater) {
        return Value(lhs.As<double>() > rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kGreaterEqual) {
        return Value(lhs.As<double>() >= rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kLessEqual) {
        return Value(lhs.As<double>() <= rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kLess) {
        return Value(lhs.As<double>() < rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kBangEqual) {
        return Value(lhs != rhs);
    } else if (expr.op_.GetType() == tokens::Type::kEqualEqual) {
        return Value(lhs == rhs);
    }

    return {};
}

Value AstInterpreter::EvaluateConditional(const expressions::Conditional& expr) const {
    auto condition = Evaluate(*expr.first_);
    if (IsTruthy(condition)) {
        return Evaluate(*expr.second_);
    } else {
        return Evaluate(*expr.third_);
    }
}

bool AstInterpreter::IsTruthy(const lox::Value& value) const {
    static constexpr auto kVisitor = [](const auto& arg) -> bool {
        using T = std::remove_cvref_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return false;
        } else if constexpr (std::is_same_v<T, bool>) {
            return arg;
        } else {
            return true;
        }
    };

    return value.Accept(kVisitor);
}

}  // namespace lox
