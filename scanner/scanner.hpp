#pragma once

#include <data_structures/tokens/tokens.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace lox {

class Lox;

class Scanner {
 public:
    Scanner(std::string&& source, Lox& lox);
    std::vector<tokens::Token> ScanTokens();

 private:
    void ScanToken();
    bool IsAtEnd() const;
    char Advance();
    void AddToken(tokens::Type type, std::optional<std::string>&& literal = std::nullopt);
    bool Match(char expected);
    char Peek() const;
    void ScanString();
    void ScanNumber();
    char PeekNext() const;
    char PeekImpl(size_t shift) const;
    void ScanIdentifierOrKeyword();
    void SkipLineComment();
    void SkipBlockComment();

    static bool IsDigit(unsigned char c);
    static bool IsAlpha(unsigned char c);
    static bool IsAlphaNumeric(unsigned char c);

 private:
    static const std::unordered_map<std::string, tokens::Type> kKeywords;

 private:
    std::string source_;
    std::vector<tokens::Token> tokens_;
    uint32_t start_ = 0;
    uint32_t current_ = 0;
    uint32_t line_ = 1;
    Lox& lox_;
};

}  // namespace lox
