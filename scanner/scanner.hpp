#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "token.hpp"

namespace lox {

class Lox;

class Scanner {
 public:
    Scanner(std::string&& source, Lox& lox);
    const std::vector<Token>& ScanTokens();

 private:
    void ScanToken();
    bool IsAtEnd() const;
    char Advance();
    void AddToken(Token::Type type);
    void AddToken(Token::Type type, std::string&& literal);
    bool Match(char expected);
    char Peek() const;
    void ScanString();
    void ScanNumber();
    char PeekNext() const;
    char PeekImpl(size_t shift) const;
    void ScanIdentifier();

 private:
    static const std::unordered_map<std::string, Token::Type> kKeywords;

 private:
    std::string source_;
    std::vector<Token> tokens_;
    uint32_t start_ = 0;
    uint32_t current_ = 0;
    uint32_t line_ = 1;
    Lox& lox_;
};

}  // namespace lox
