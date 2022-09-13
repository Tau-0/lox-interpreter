#include "expressions.hpp"

namespace lox::expressions {

const std::string& String::Value() const {
    return value_;
}

double Number::Value() const {
    return value_;
}

bool Boolean::Value() const {
    return value_;
}

const tokens::Token& Unary::Operation() const {
    return op_;
}

const Expr& Unary::Expression() const {
    return *expr_;
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

const Expr& Grouping::Expression() const {
    return *expr_;
}

}  // namespace lox::expressions
