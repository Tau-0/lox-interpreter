#pragma once

#include <data_structures/ast/expressions.hpp>
#include <variant>

namespace lox::statements {

struct Expression {
    explicit Expression(expressions::ExprPtr expr);

    expressions::ExprPtr expr_;
};

struct Print {
    explicit Print(expressions::ExprPtr expr);

    expressions::ExprPtr expr_;
};

struct Var {
    Var(tokens::Token&& name, expressions::ExprPtr initializer);

    tokens::Token name_;
    expressions::ExprPtr initializer_;
};

class Stmt {
 public:
    Stmt() = default;

    template <typename T>
    explicit Stmt(T&& stmt) : stmt_(std::forward<T>(stmt)) {
    }

    template <typename V>
    auto Accept(V& visitor) const {
        return std::visit(visitor, stmt_);
    }

    template <typename T>
    bool Is() const {
        return std::holds_alternative<T>(stmt_);
    }

 private:
    std::variant<std::monostate, Expression, Print, Var> stmt_;
};

template <typename T, typename... Args>
Stmt MakeStmt(Args&&... args) {
    return Stmt(T(std::forward<Args>(args)...));
}

template <typename T>
concept IsStatement = IsTypeOf<T, std::monostate, Expression, Print, Var>;

}  // namespace lox::statements
