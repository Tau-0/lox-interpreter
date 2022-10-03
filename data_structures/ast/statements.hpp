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

class Stmt {
 public:
    template <typename T>
    explicit Stmt(T&& stmt) : stmt_(std::forward<T>(stmt)) {
    }

    template <typename V>
    auto Accept(const V& visitor) const {
        return std::visit(visitor, stmt_);
    }

 private:
    std::variant<Expression, Print> stmt_;
};

template <typename T, typename... Args>
Stmt MakeStmt(Args&&... args) {
    return Stmt(T(std::forward<Args>(args)...));
}

template <typename T>
concept IsStatement = IsTypeOf<T, Expression, Print>;

}  // namespace lox::statements
