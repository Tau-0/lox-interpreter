#pragma once

#include <string>

namespace lox::tokens {

enum class Type {
    kLeftParen,
    kRightParen,
    kLeftBrace,
    kRightBrace,
    kColon,
    kComma,
    kDot,
    kMinus,
    kPlus,
    kQuestion,
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
bool IsArithmetic(Type type);
bool IsComparison(Type type);

}  // namespace lox::tokens
