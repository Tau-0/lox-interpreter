#pragma once

#include <string>

namespace lox::tokens {

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

std::string AsString(Type type);

}  // namespace lox::tokens
