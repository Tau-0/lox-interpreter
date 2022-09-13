#pragma once

#include <data_structures/ast/expr_visitor.hpp>
#include <data_structures/ast/expressions.hpp>

namespace lox {

class AstPrinter : public expressions::IExprVisitor<std::string> {
 public:
    std::string Print(const expressions::Expr& expr) const;
    std::string Visit(const expressions::String& expr) const override;
    std::string Visit(const expressions::Number& expr) const override;
    std::string Visit(const expressions::Boolean& expr) const override;
    std::string Visit(const expressions::Nil& expr) const override;
    std::string Visit(const expressions::Unary& expr) const override;
    std::string Visit(const expressions::Binary& expr) const override;
    std::string Visit(const expressions::Grouping& expr) const override;

    // temporary
    void Test() const;

 private:
    template <typename T>
    std::string ParenthesizeImpl(const T& t) const {
        return " " + t.Accept(*this);
    }

    template <typename T, typename... Args>
    std::string ParenthesizeImpl(const T& t, const Args&... args) const {
        return ParenthesizeImpl(t) + ParenthesizeImpl(args...);
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
