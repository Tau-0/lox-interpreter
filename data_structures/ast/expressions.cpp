#include "expressions.hpp"

namespace lox::expressions {

String::String(std::string&& value) : value_(std::move(value)) {
}

const std::string& String::Value() const {
    return value_;
}

Number::Number(double value) : value_(value) {
}

double Number::Value() const {
    return value_;
}

Boolean::Boolean(bool value) : value_(value) {
}

bool Boolean::Value() const {
    return value_;
}

Unary::Unary(lox::expressions::ExprPtr expr, tokens::Token&& op) : expr_(std::move(expr)), op_(std::move(op)) {
}

const tokens::Token& Unary::Operation() const {
    return op_;
}

const Expr& Unary::Expression() const {
    return *expr_;
}

Binary::Binary(lox::expressions::ExprPtr left, lox::expressions::ExprPtr right, tokens::Token&& op)
    : left_(std::move(left)), right_(std::move(right)), op_(std::move(op)) {
}

const tokens::Token& Binary::Operation() const {
    return op_;
}

const Expr& Binary::LeftExpression() const {
    return *left_;
}

const Expr& Binary::RightExpression() const {
    return *right_;
}

Grouping::Grouping(lox::expressions::ExprPtr expr) : expr_(std::move(expr)) {
}

const Expr& Grouping::Expression() const {
    return *expr_;
}

}  // namespace lox::expressions
