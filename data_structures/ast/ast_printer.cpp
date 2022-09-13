#include "ast_printer.hpp"

#include <iostream>

namespace lox {

std::string AstPrinter::Print(const expressions::Expr& expr) const {
    return expr.Accept(*this);
}

std::string AstPrinter::Visit(const expressions::String& expr) const {
    return expr.Value();
}

std::string AstPrinter::Visit(const expressions::Number& expr) const {
    return std::to_string(expr.Value());
}

std::string AstPrinter::Visit(const expressions::Boolean& expr) const {
    return expr.Value() ? "true" : "false";
}

std::string AstPrinter::Visit(const expressions::Nil& expr) const {
    return "nil";
}

std::string AstPrinter::Visit(const expressions::Unary& expr) const {
    return Parenthesize(expr.Operation().Lexeme(), expr.Expression());
}

std::string AstPrinter::Visit(const expressions::Binary& expr) const {
    return Parenthesize(expr.Operation().Lexeme(), expr.LeftExpression(), expr.RightExpression());
}

std::string AstPrinter::Visit(const expressions::Grouping& expr) const {
    return Parenthesize("group", expr.Expression());
}

void AstPrinter::Test() const {
    using expressions::Binary;
    using expressions::Grouping;
    using expressions::MakeExpr;
    using expressions::Number;
    using expressions::Unary;
    using tokens::MakeToken;
    using tokens::NonLiteral;

    auto expr = MakeExpr<Binary>(                                                                     //
        MakeExpr<Unary>(MakeExpr<Number>(123), MakeToken<NonLiteral>(tokens::Type::kMinus, "-", 1)),  //
        MakeExpr<Grouping>(MakeExpr<Number>(45.67)),                                                  //
        MakeToken<NonLiteral>(tokens::Type::kStar, "*", 1)                                            //
    );

    std::cout << Print(*expr) << "\n";
}

}  // namespace lox
