#pragma once

#include <memory>

namespace lox::expressions {

class String;
class Number;
class Boolean;
class Nil;
class Unary;
class Binary;
class Grouping;
class Expr;

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

}  // namespace lox::expressions
