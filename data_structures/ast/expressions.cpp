#include "expressions.hpp"

namespace lox::expressions {

String::String(std::string value) : value_(std::move(value)) {
}

Number::Number(double value) : value_(value) {
}

Boolean::Boolean(bool value) : value_(value) {
}

Unary::Unary(lox::expressions::ExprPtr expr, tokens::Token&& op) : expr_(std::move(expr)), op_(std::move(op)) {
}

Binary::Binary(lox::expressions::ExprPtr left, lox::expressions::ExprPtr right, tokens::Token&& op)
    : left_(std::move(left)), right_(std::move(right)), op_(std::move(op)) {
}

TernaryConditional::TernaryConditional(ExprPtr first, ExprPtr second, ExprPtr third)
    : first_(std::move(first)), second_(std::move(second)), third_(std::move(third)) {
}

Grouping::Grouping(lox::expressions::ExprPtr expr) : expr_(std::move(expr)) {
}

}  // namespace lox::expressions
