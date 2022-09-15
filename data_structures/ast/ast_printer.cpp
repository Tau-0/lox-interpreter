#include "ast_printer.hpp"

#include <iostream>

namespace lox {

std::string AstPrinter::Print(const expressions::Expr& expr) const {
    return expr.Accept(*this);
}

std::string AstPrinter::Visit(const expressions::String& expr) const {
    return expr.GetValue();
}

std::string AstPrinter::Visit(const expressions::Number& expr) const {
    return std::to_string(expr.GetValue());
}

std::string AstPrinter::Visit(const expressions::Boolean& expr) const {
    return expr.GetValue() ? "true" : "false";
}

std::string AstPrinter::Visit(const expressions::Nil& expr) const {
    return "nil";
}

std::string AstPrinter::Visit(const expressions::Unary& expr) const {
    return Parenthesize(expr.GetOperation().GetLexeme(), expr.GetExpression());
}

std::string AstPrinter::Visit(const expressions::Binary& expr) const {
    return Parenthesize(expr.GetOperation().GetLexeme(), expr.GetLeftExpression(), expr.GetRightExpression());
}

std::string AstPrinter::Visit(const expressions::Grouping& expr) const {
    return Parenthesize("group", expr.GetExpression());
}

}  // namespace lox
