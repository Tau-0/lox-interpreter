#include "type.hpp"

#include <unordered_map>

namespace lox::tokens {

const std::unordered_map<Type, std::string> kTypeStrings = {
    {Type::kLeftParen, "("},              //
    {Type::kRightParen, ")"},             //
    {Type::kLeftBrace, "{"},              //
    {Type::kRightBrace, "}"},             //
    {Type::kColon, ":"},                  //
    {Type::kComma, ","},                  //
    {Type::kDot, "."},                    //
    {Type::kMinus, "-"},                  //
    {Type::kPlus, "+"},                   //
    {Type::kQuestion, "?"},               //
    {Type::kSemicolon, ";"},              //
    {Type::kSlash, "/"},                  //
    {Type::kStar, "*"},                   //
    {Type::kBang, "!"},                   //
    {Type::kBangEqual, "!="},             //
    {Type::kEqual, "="},                  //
    {Type::kEqualEqual, "=="},            //
    {Type::kGreater, ">"},                //
    {Type::kGreaterEqual, ">="},          //
    {Type::kLess, "<"},                   //
    {Type::kLessEqual, "<="},             //
    {Type::kIdentifier, "Identifier"},    //
    {Type::kString, "Literal (String)"},  //
    {Type::kNumber, "Literal (Number)"},  //
    {Type::kAnd, "Keyword (and)"},        //
    {Type::kClass, "Keyword (class)"},    //
    {Type::kElse, "Keyword (else)"},      //
    {Type::kFalse, "Keyword (false)"},    //
    {Type::kFun, "Keyword (fun)"},        //
    {Type::kFor, "Keyword (for)"},        //
    {Type::kIf, "Keyword (if)"},          //
    {Type::kNil, "Keyword (nil)"},        //
    {Type::kOr, "Keyword (or)"},          //
    {Type::kPrint, "Keyword (print)"},    //
    {Type::kReturn, "Keyword (return)"},  //
    {Type::kSuper, "Keyword (super)"},    //
    {Type::kThis, "Keyword (this)"},      //
    {Type::kTrue, "Keyword (true)"},      //
    {Type::kVar, "Keyword (var)"},        //
    {Type::kWhile, "Keyword (while)"},    //
    {Type::kEof, "EOF"},                  //
};

std::string AsString(Type type) {
    return kTypeStrings.at(type);
}

}  // namespace lox::tokens
