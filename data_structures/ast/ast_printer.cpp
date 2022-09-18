#include "ast_printer.hpp"

#include <iostream>

namespace lox {

std::string AstPrinter::Print(const expressions::Expr& expr) const {
    return expr.Accept(*this);
}

}  // namespace lox
