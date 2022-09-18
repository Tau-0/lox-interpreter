#include "tokens.hpp"

namespace lox::tokens {

std::string Base::ToString() const {
    std::string buffer = "Type: " + AsString(type_);
    if (!lexeme_.empty()) {
        buffer += ", Lexeme: " + lexeme_;
    }
    return buffer + ", Line: " + std::to_string(line_);
}

Base::Base(Type type, std::string&& lexeme, uint32_t line) : lexeme_(std::move(lexeme)), type_(type), line_(line) {
}

String::String(lox::tokens::Type type, std::string&& lexeme, std::string&& literal, uint32_t line)
    : LiteralBase(type, std::move(lexeme), std::move(literal), line) {
}

std::string String::ToString() const {
    return LiteralBase::ToString() + literal_;
}

Number::Number(lox::tokens::Type type, std::string&& lexeme, double literal, uint32_t line)
    : LiteralBase(type, std::move(lexeme), std::move(literal), line) {
}

std::string Number::ToString() const {
    return LiteralBase::ToString() + std::to_string(literal_);
}

NonLiteral::NonLiteral(lox::tokens::Type type, std::string&& lexeme, uint32_t line)
    : Base(type, std::move(lexeme), line) {
}

std::string Token::ToString() const {
    static constexpr auto kVisitor = [](const auto& arg) -> std::string {
        return arg.ToString();
    };

    return std::visit(kVisitor, token_);
}

const std::string& Token::GetLexeme() const {
    static constexpr auto kVisitor = [](const auto& arg) -> const std::string& {
        return arg.lexeme_;
    };

    return std::visit(kVisitor, token_);
}

Type Token::GetType() const {
    static constexpr auto kVisitor = [](const auto& arg) -> Type {
        return arg.type_;
    };

    return std::visit(kVisitor, token_);
}

}  // namespace lox::tokens
