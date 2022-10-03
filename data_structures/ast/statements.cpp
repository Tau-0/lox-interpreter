#include "statements.hpp"

namespace lox::statements {

Expression::Expression(expressions::ExprPtr expr) : expr_(std::move(expr)) {
}

Print::Print(expressions::ExprPtr expr) : expr_(std::move(expr)) {
}

}  // namespace lox::statements
