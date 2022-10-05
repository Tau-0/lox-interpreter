#pragma once

#include <data_structures/ast/expressions.hpp>
#include <variant>
#include <vector>

namespace lox::statements {

class Stmt;

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

struct Block {
    explicit Block(std::vector<Stmt>&& statements);

    std::vector<Stmt> statements_;
};

template <typename T>
concept IsStatement = IsTypeOf<T, std::monostate, Expression, Print, Var, Block>;

class Stmt {
 public:
    Stmt() = default;

    template <IsStatement T>
    explicit Stmt(T&& stmt) : stmt_(std::forward<T>(stmt)) {
    }

    template <typename V>
    auto Accept(V& visitor) const {
        return std::visit(visitor, stmt_);
    }

    template <IsStatement T>
    bool Is() const {
        return std::holds_alternative<T>(stmt_);
    }

 private:
    std::variant<std::monostate, Expression, Print, Var, Block> stmt_;
};

template <IsStatement T, typename... Args>
Stmt MakeStmt(Args&&... args) {
    return Stmt(T(std::forward<Args>(args)...));
}

}  // namespace lox::statements
