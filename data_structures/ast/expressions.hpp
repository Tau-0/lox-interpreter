#pragma once

#include <data_structures/tokens/tokens.hpp>
#include <memory>
#include <string>
#include <variant>

namespace lox::expressions {

class String;
class Number;
class Boolean;
class Nil;
class Unary;
class Binary;
class Grouping;
class Expr;

using ExprPtr = std::shared_ptr<Expr>;

template <typename T>
struct IExprVisitor {
    virtual T Visit(const String&) const = 0;
    virtual T Visit(const Number&) const = 0;
    virtual T Visit(const Boolean&) const = 0;
    virtual T Visit(const Nil&) const = 0;
    virtual T Visit(const Unary&) const = 0;
    virtual T Visit(const Binary&) const = 0;
    virtual T Visit(const Grouping&) const = 0;
    virtual ~IExprVisitor() = default;
};

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
    const std::string& Value() const;

 private:
    std::string value_;
};

class Number : public ExprBase<Number> {
 public:
    double Value() const;

 private:
    double value_ = 0.0;
};

class Boolean : public ExprBase<Boolean> {
 public:
    bool Value() const;

 private:
    bool value_ = false;
};

class Nil : public ExprBase<Nil> {
 public:
 private:
    std::monostate value_;
};

class Unary : public ExprBase<Unary> {
 public:
    const tokens::Token& Operation() const;
    const Expr& Expression() const;

 private:
    ExprPtr expr_;
    tokens::Token op_;
};

class Binary : public ExprBase<Binary> {
 public:
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

}  // namespace lox::expressions
