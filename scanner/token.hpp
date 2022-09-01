#pragma once

#include <string>

namespace lox {

class Token {
 public:
    enum class Type {
        kLeftParen,
        kRightParen,
        kLeftBrace,
        kRightBrace,
        kComma,
        kDot,
        kMinus,
        kPlus,
        kSemicolon,
        kSlash,
        kStar,

        kBang,
        kBangEqual,
        kEqual,
        kEqualEqual,
        kGreater,
        kGreaterEqual,
        kLess,
        kLessEqual,

        kIdentifier,
        kString,
        kNumber,

        kAnd,
        kClass,
        kElse,
        kFalse,
        kFun,
        kFor,
        kIf,
        kNil,
        kOr,
        kPrint,
        kReturn,
        kSuper,
        kThis,
        kTrue,
        kVar,
        kWhile,
        kEof,
    };

 public:
    Token(Type type, std::string&& lexeme, std::string&& literal, uint32_t line);
    std::string ToString() const;

 private:
    Type type_;
    std::string lexeme_;
    std::string literal_;
    uint32_t line_ = 0;
};

}  // namespace lox
