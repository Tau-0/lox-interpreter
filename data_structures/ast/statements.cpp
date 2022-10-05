#include "statements.hpp"

namespace lox::statements {

Expression::Expression(expressions::ExprPtr expr) : expr_(std::move(expr)) {
}

Print::Print(expressions::ExprPtr expr) : expr_(std::move(expr)) {
}

Var::Var(tokens::Token&& name, expressions::ExprPtr initializer)
    : name_(std::move(name)), initializer_(std::move(initializer)) {
}

Block::Block(std::vector<Stmt>&& statements) : statements_(std::move(statements)) {
}

If::If(expressions::ExprPtr condition, StmtPtr then_branch, StmtPtr else_branch)
    : condition_(std::move(condition)), then_branch_(std::move(then_branch)), else_branch_(std::move(else_branch)) {
}

While::While(expressions::ExprPtr condition, lox::statements::StmtPtr statement)
    : condition_(std::move(condition)), statement_(std::move(statement)) {
}

}  // namespace lox::statements
