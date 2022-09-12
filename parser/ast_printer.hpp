#pragma once

#include "expressions/expressions.hpp"

namespace lox {

class AstPrinter : public expressions::IExprVisitor<std::string> {
 public:
    std::string Visit(const expressions::String& expr) const override {
        return expr.Value();
    }

    std::string Visit(const expressions::Number& expr) const override {
        return std::to_string(expr.Value());
    }

    std::string Visit(const expressions::Boolean& expr) const override {
        return expr.Value() ? "true" : "false";
    }

    std::string Visit(const expressions::Nil& expr) const override {
        return "nil";
    }

    std::string Visit(const expressions::Unary& expr) const override {
        return Parenthesize(expr.Operation().Lexeme(), expr.Expression());
    }

    std::string Visit(const expressions::Binary& expr) const override {
        return Parenthesize(expr.Operation().Lexeme(), expr.LeftExpression(), expr.RightExpression());
    }

    std::string Visit(const expressions::Grouping& expr) const override {
        return Parenthesize("group", expr.Expression());
    }

    // temporary
    //    void Test() const {
    //        expressions::Binary expr(std::make_shared<expressions::Binary>());
    //    }

 private:
    template <typename T>
    std::string ParenthesizeImpl(const T& t) const {
        return t.Accept(*this);
    }

    template <typename T, typename... Args>
    std::string ParenthesizeImpl(const T& t, const Args&... args) const {
        return " " + ParenthesizeImpl(t) + " " + ParenthesizeImpl(args...);
    }

    template <typename... Args>
    std::string Parenthesize(const std::string& name, const Args&... args) const {
        std::string result = "(" + name;
        result += ParenthesizeImpl(args...);
        result += ")";
        return result;
    }
};

}  // namespace lox
