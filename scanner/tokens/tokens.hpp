#pragma once

#include <string>
#include <variant>

#include "literal_base.hpp"
#include "token_base.hpp"

namespace lox::tokens {

namespace literals {

class String : public LiteralBase<String, std::string> {
 private:
    friend LiteralBase<String, std::string>;

 public:
    String(Type type, std::string&& lexeme, std::string&& literal, uint32_t line);

 private:
    std::string ToStringImpl() const;
};

class Number : public LiteralBase<Number, double> {
 private:
    friend LiteralBase<Number, double>;

 public:
    Number(Type type, std::string&& lexeme, double literal, uint32_t line);

 private:
    std::string ToStringImpl() const;
};

}  // namespace literals

class NonLiteral : public Base<NonLiteral> {
 public:
    NonLiteral(Type type, std::string&& lexeme, uint32_t line);
};

class Token {
 public:
    template <typename T>
    explicit Token(T&& token) : token_(std::forward<T>(token)) {
    }

    std::string ToString() const;
    const std::string& Lexeme() const;

 private:
    std::variant<NonLiteral, literals::Number, literals::String> token_;
};

}  // namespace lox::tokens
