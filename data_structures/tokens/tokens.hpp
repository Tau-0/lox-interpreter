#pragma once

#include <data_structures/tokens/type.hpp>
#include <string>
#include <variant>

namespace lox::tokens {

struct Base {
 public:
    std::string ToString() const;

 protected:
    Base(Type type, std::string&& lexeme, uint32_t line);

 public:
    std::string lexeme_;
    Type type_;
    uint32_t line_;
};

template <typename T>
struct LiteralBase : public Base {
 protected:
    LiteralBase(Type type, std::string&& lexeme, T&& literal, uint32_t line)
        : Base(type, std::move(lexeme), line), literal_(std::move(literal)) {
    }

    std::string ToString() const {
        return Base::ToString() + ", Literal: ";
    }

 public:
    T literal_;
};

struct String : public LiteralBase<std::string> {
    String(Type type, std::string&& lexeme, std::string&& literal, uint32_t line);
    std::string ToString() const;
};

struct Number : public LiteralBase<double> {
    Number(Type type, std::string&& lexeme, double literal, uint32_t line);
    std::string ToString() const;
};

struct NonLiteral : public Base {
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
    std::variant<NonLiteral, Number, String> token_;
};

template <typename T, typename... Args>
Token MakeToken(Args&&... args) {
    return Token(T(std::forward<Args>(args)...));
}

}  // namespace lox::tokens
