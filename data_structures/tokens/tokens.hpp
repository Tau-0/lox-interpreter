#pragma once

#include <data_structures/tokens/literal_base.hpp>
#include <data_structures/tokens/token_base.hpp>
#include <string>
#include <variant>

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

    template <typename T>
    const T& As() const {
        return get<T>(token_);
    }

    std::string ToString() const;
    const std::string& GetLexeme() const;
    Type GetType() const;

 private:
    std::variant<NonLiteral, literals::Number, literals::String> token_;
};

template <typename T, typename... Args>
Token MakeToken(Args&&... args) {
    return Token(T(std::forward<Args>(args)...));
}

}  // namespace lox::tokens
