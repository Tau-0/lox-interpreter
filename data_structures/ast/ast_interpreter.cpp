#include "ast_interpreter.hpp"

#include <cassert>
#include <lox/errors.hpp>
#include <lox/lox.hpp>

namespace lox {

AstInterpreter::AstInterpreter(lox::Lox& lox) : lox_(lox) {
}

Value AstInterpreter::Interpret(const expressions::Expr& expr) const {
    try {
        return Evaluate(expr);
    } catch (const RuntimeError& error) {
        lox_.RuntimeError(error);
    }
    return {};
}

Value AstInterpreter::Evaluate(const expressions::Expr& expr) const {
    return expr.Accept(*this);
}

Value AstInterpreter::EvaluateUnary(const expressions::Unary& expr) const {
    Value rhs = Evaluate(*expr.expr_);
    if (expr.op_.GetType() == tokens::Type::kMinus) {
        CheckNumberOperand(expr.op_, rhs);
        return Value(-rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kBang) {
        return Value(!IsTruthy(rhs));
    }
    return rhs;
}

Value AstInterpreter::EvaluateBinary(const expressions::Binary& expr) const {
    Value lhs = Evaluate(*expr.left_);
    Value rhs = Evaluate(*expr.right_);
    if (expr.op_.GetType() == tokens::Type::kPlus) {
        return SumOrConcatenate(expr.op_, lhs, rhs);
    } else if (tokens::IsArithmetic(expr.op_.GetType()) || tokens::IsComparison(expr.op_.GetType())) {
        CheckNumberOperands(expr.op_, lhs, rhs);
        return NumberOperation(expr.op_, lhs.As<double>(), rhs.As<double>());
    } else if (expr.op_.GetType() == tokens::Type::kBangEqual) {
        return Value(lhs != rhs);
    } else if (expr.op_.GetType() == tokens::Type::kEqualEqual) {
        return Value(lhs == rhs);
    } else {
        // Unreachable
        assert(false && "Unknown operation");
        return {};
    }
}

Value AstInterpreter::EvaluateConditional(const expressions::Conditional& expr) const {
    auto condition = Evaluate(*expr.first_);
    if (IsTruthy(condition)) {
        return Evaluate(*expr.second_);
    } else {
        return Evaluate(*expr.third_);
    }
}

Value AstInterpreter::NumberOperation(const tokens::Token& op, double lhs, double rhs) const {
    auto type = op.GetType();
    if (type == tokens::Type::kMinus) {
        return Value(lhs - rhs);
    } else if (type == tokens::Type::kStar) {
        return Value(lhs * rhs);
    } else if (type == tokens::Type::kSlash) {
        if (rhs == 0) {
            throw RuntimeError(op, "Division by zero.");
        }
        return Value(lhs / rhs);
    } else if (type == tokens::Type::kGreater) {
        return Value(lhs > rhs);
    } else if (type == tokens::Type::kGreaterEqual) {
        return Value(lhs >= rhs);
    } else if (type == tokens::Type::kLessEqual) {
        return Value(lhs <= rhs);
    } else if (type == tokens::Type::kLess) {
        return Value(lhs < rhs);
    } else {
        // Unreachable
        assert(false && "Unknown operation");
        return {};
    }
}

Value AstInterpreter::SumOrConcatenate(const tokens::Token& op, const lox::Value& lhs, const lox::Value& rhs) const {
    if (lhs.Is<std::string>() && rhs.Is<std::string>()) {
        return Value(lhs.As<std::string>() + rhs.As<std::string>());
    } else if (lhs.Is<double>() && rhs.Is<double>()) {
        return Value(lhs.As<double>() + rhs.As<double>());
    } else {
        throw RuntimeError(op, "Operands must be two numbers or two strings.");
    }
}

bool AstInterpreter::IsTruthy(const lox::Value& value) const {
    static constexpr auto kVisitor = [](const auto& arg) -> bool {
        using T = std::decay_t<decltype(arg)>;
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

void AstInterpreter::CheckNumberOperand(const tokens::Token& op, const lox::Value& value) const {
    if (!value.Is<double>()) {
        throw RuntimeError(op, "Operand must be a number.");
    }
}

void AstInterpreter::CheckNumberOperands(const tokens::Token& op, const lox::Value& lhs, const lox::Value& rhs) const {
    if (!lhs.Is<double>() || !rhs.Is<double>()) {
        throw RuntimeError(op, "Operands must be numbers.");
    }
}

}  // namespace lox
