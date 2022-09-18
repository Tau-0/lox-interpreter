#pragma once

#include <data_structures/tokens/tokens.hpp>
#include <memory>
#include <string>
#include <variant>

namespace lox::expressions {

struct String;
struct Number;
struct Boolean;
struct Nil;
struct Unary;
struct Binary;
struct Grouping;
class Expr;

using ExprPtr = std::shared_ptr<Expr>;

struct String {
    explicit String(std::string value);

    std::string value_;
};

struct Number {
    explicit Number(double value);

    double value_ = 0.0;
};

struct Boolean {
    explicit Boolean(bool value);

    bool value_ = false;
};

struct Nil {  //
};

struct Unary {
    Unary(ExprPtr expr, tokens::Token&& op);

    ExprPtr expr_;
    tokens::Token op_;
};

struct Binary {
    Binary(ExprPtr left, ExprPtr right, tokens::Token&& op);

    ExprPtr left_;
    ExprPtr right_;
    tokens::Token op_;
};

struct Grouping {
    explicit Grouping(ExprPtr expr);

    ExprPtr expr_;
};

class Expr {
 public:
    template <typename T>
    explicit Expr(T&& expr) : expr_(std::forward<T>(expr)) {
    }

    template <typename V>
    auto Accept(const V& visitor) const {
        return std::visit(visitor, expr_);
    }

 private:
    std::variant<String, Number, Boolean, Nil, Unary, Binary, Grouping> expr_;
};

template <typename T, typename... Args>
ExprPtr MakeExpr(Args&&... args) {
    return std::make_shared<Expr>(T(std::forward<Args>(args)...));
}

}  // namespace lox::expressions
