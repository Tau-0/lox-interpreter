//#pragma once
//
//namespace lox::ast {
//
//class String;
//class Number;
//class Boolean;
//class Nil;
//class Unary;
//class Binary;
//class Grouping;
//
//struct IExprVisitor;
//
//template <typename T>
//struct IExprVisitor {
//    virtual ~IExprVisitor() = default;
//    virtual T Visit(String&) = 0;
//    virtual T Visit(Number&) = 0;
//    virtual T Visit(Boolean&) = 0;
//    virtual T Visit(Nil&) = 0;
//    virtual T Visit(Unary&) = 0;
//    virtual T Visit(Binary&) = 0;
//    virtual T Visit(Grouping&) = 0;
//};
//
//}  // namespace lox::ast
