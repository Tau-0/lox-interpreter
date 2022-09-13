#pragma once

#include <data_structures/ast/expr_visitor.hpp>
#include <data_structures/tokens/tokens.hpp>
#include <memory>
#include <string>
#include <variant>

namespace lox::expressions {

using ExprPtr = std::shared_ptr<Expr>;

template <typename C>
class ExprBase {
 public:
    template <typename T>
    T Accept(const IExprVisitor<T>& visitor) const {
        return visitor.Visit(*AsHeir());
    }

 private:
    C* AsHeir() {
        return static_cast<C*>(this);
    }

    const C* AsHeir() const {
        return static_cast<const C*>(this);
    }
};

class String : public ExprBase<String> {
 public:
    explicit String(std::string&& value);
    const std::string& Value() const;

 private:
    std::string value_;
};

class Number : public ExprBase<Number> {
 public:
    explicit Number(double value);
    double Value() const;

 private:
    double value_ = 0.0;
};

class Boolean : public ExprBase<Boolean> {
 public:
    explicit Boolean(bool value);
    bool Value() const;

 private:
    bool value_ = false;
};

class Nil : public ExprBase<Nil> {
 private:
    std::monostate value_;
};

class Unary : public ExprBase<Unary> {
 public:
    Unary(ExprPtr expr, tokens::Token&& op);
    const tokens::Token& Operation() const;
    const Expr& Expression() const;

 private:
    ExprPtr expr_;
    tokens::Token op_;
};

class Binary : public ExprBase<Binary> {
 public:
    Binary(ExprPtr left, ExprPtr right, tokens::Token&& op);
    const tokens::Token& Operation() const;
    const Expr& LeftExpression() const;
    const Expr& RightExpression() const;

 private:
    ExprPtr left_;
    ExprPtr right_;
    tokens::Token op_;
};

class Grouping : public ExprBase<Grouping> {
 public:
    explicit Grouping(ExprPtr expr);
    const Expr& Expression() const;

 private:
    ExprPtr expr_;
};

class Expr {
 public:
    template <typename T>
    explicit Expr(T&& expr) : expr_(std::forward<T>(expr)) {
    }

    template <typename T>
    T Accept(const IExprVisitor<T>& visitor) const {
        auto v = [&visitor](const auto& arg) -> T {
            return arg.Accept(visitor);
        };
        return std::visit(std::move(v), expr_);
    }

 private:
    std::variant<String, Number, Boolean, Nil, Unary, Binary, Grouping> expr_;
};

template <typename T, typename... Args>
ExprPtr MakeExpr(Args&&... args) {
    return std::make_shared<Expr>(T(std::forward<Args>(args)...));
}

}  // namespace lox::expressions
