#include "token.hpp"

namespace lox {

Token::Token(Token::Type type, std::string&& lexeme, std::string&& literal, uint32_t line)
    : type_(type), lexeme_(std::move(lexeme)), literal_(std::move(literal)), line_(line) {
}

std::string Token::ToString() const {
    return "line: " + std::to_string(line_) + ": lexeme " + lexeme_ + " with literal " + literal_;
}

}  // namespace lox
