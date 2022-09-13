#include "tokens.hpp"

namespace lox::tokens {

namespace literals {

String::String(lox::tokens::Type type, std::string&& lexeme, std::string&& literal, uint32_t line)
    : LiteralBase<String, std::string>(type, std::move(lexeme), std::move(literal), line) {
}

std::string String::ToStringImpl() const {
    return literal_;
}

Number::Number(lox::tokens::Type type, std::string&& lexeme, double literal, uint32_t line)
    : LiteralBase<Number, double>(type, std::move(lexeme), std::move(literal), line) {
}

std::string Number::ToStringImpl() const {
    return std::to_string(literal_);
}

}  // namespace literals

NonLiteral::NonLiteral(lox::tokens::Type type, std::string&& lexeme, uint32_t line)
    : Base<NonLiteral>(type, std::move(lexeme), line) {
}

std::string Token::ToString() const {
    static constexpr auto kVisitor = [](const auto& arg) -> std::string {
        return arg.ToString();
    };

    return std::visit(kVisitor, token_);
}

const std::string& Token::Lexeme() const {
    static constexpr auto kVisitor = [](const auto& arg) -> const std::string& {
        return arg.Lexeme();
    };

    return std::visit(kVisitor, token_);
}

}  // namespace lox::tokens
