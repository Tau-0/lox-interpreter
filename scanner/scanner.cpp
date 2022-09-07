#include "scanner.hpp"

#include <cassert>

#include "../lox.hpp"

namespace lox {

const std::unordered_map<std::string, tokens::Type> Scanner::kKeywords = {
    {"and", tokens::Type::kAnd},        //
    {"class", tokens::Type::kClass},    //
    {"else", tokens::Type::kElse},      //
    {"false", tokens::Type::kFalse},    //
    {"for", tokens::Type::kFor},        //
    {"fun", tokens::Type::kFun},        //
    {"if", tokens::Type::kIf},          //
    {"nil", tokens::Type::kNil},        //
    {"or", tokens::Type::kOr},          //
    {"print", tokens::Type::kPrint},    //
    {"return", tokens::Type::kReturn},  //
    {"super", tokens::Type::kSuper},    //
    {"this", tokens::Type::kThis},      //
    {"true", tokens::Type::kTrue},      //
    {"var", tokens::Type::kVar},        //
    {"while", tokens::Type::kWhile},    //
};

Scanner::Scanner(std::string&& source, Lox& lox) : source_(std::move(source)), lox_(lox) {
}

const std::vector<tokens::Token>& Scanner::ScanTokens() {
    while (!IsAtEnd()) {
        start_ = current_;
        ScanToken();
    }
    tokens_.emplace_back(tokens::NonLiteral(tokens::Type::kEof, "", line_));
    return tokens_;
}

void Scanner::ScanToken() {
    char c = Advance();
    if (c == '(') {
        AddNonLiteralToken(tokens::Type::kLeftParen);
    } else if (c == ')') {
        AddNonLiteralToken(tokens::Type::kRightParen);
    } else if (c == '{') {
        AddNonLiteralToken(tokens::Type::kLeftBrace);
    } else if (c == '}') {
        AddNonLiteralToken(tokens::Type::kRightBrace);
    } else if (c == ',') {
        AddNonLiteralToken(tokens::Type::kComma);
    } else if (c == '.') {
        AddNonLiteralToken(tokens::Type::kDot);
    } else if (c == '-') {
        AddNonLiteralToken(tokens::Type::kMinus);
    } else if (c == '+') {
        AddNonLiteralToken(tokens::Type::kPlus);
    } else if (c == ';') {
        AddNonLiteralToken(tokens::Type::kSemicolon);
    } else if (c == '*') {
        AddNonLiteralToken(tokens::Type::kStar);
    } else if (c == '!') {
        AddNonLiteralToken(Match('=') ? tokens::Type::kBangEqual : tokens::Type::kBang);
    } else if (c == '=') {
        AddNonLiteralToken(Match('=') ? tokens::Type::kEqualEqual : tokens::Type::kEqual);
    } else if (c == '<') {
        AddNonLiteralToken(Match('=') ? tokens::Type::kLessEqual : tokens::Type::kLess);
    } else if (c == '>') {
        AddNonLiteralToken(Match('=') ? tokens::Type::kGreaterEqual : tokens::Type::kGreater);
    } else if (c == '/') {
        if (Match('/')) {
            while (Peek() != '\n' && !IsAtEnd()) {
                Advance();
            }
        } else {
            AddNonLiteralToken(tokens::Type::kSlash);
        }
    } else if (c == ' ' || c == '\r' || c == '\t') {
        // Skip
        return;
    } else if (c == '\n') {
        ++line_;
    } else if (c == '"') {
        ScanString();
    } else if (std::isdigit(c) != 0) {
        ScanNumber();
    } else if (std::isalpha(c) != 0 || c == '_') {
        ScanIdentifier();
    } else {
        lox_.Error(line_, "Unexpected character.");
    }
}

bool Scanner::IsAtEnd() const {
    return current_ >= source_.length();
}

char Scanner::Advance() {
    return source_[current_++];
}

void Scanner::AddNonLiteralToken(tokens::Type type) {
    std::string lexeme = source_.substr(start_, current_ - start_);
    tokens_.emplace_back(tokens::NonLiteral(type, std::move(lexeme), line_));
}

void Scanner::AddLiteralToken(tokens::Type type, std::string&& literal) {
    std::string lexeme = source_.substr(start_, current_ - start_);
    if (type == tokens::Type::kNumber) {
        tokens_.emplace_back(tokens::literals::Number(type, std::move(lexeme), std::stod(literal), line_));
    } else {
        tokens_.emplace_back(tokens::literals::String(type, std::move(lexeme), std::move(literal), line_));
    }
}

bool Scanner::Match(char expected) {
    if (Peek() != expected) {
        return false;
    }
    ++current_;
    return true;
}

char Scanner::PeekImpl(size_t shift) const {
    if (current_ + shift >= source_.length()) {
        return '\0';
    }
    return source_[current_ + shift];
}

char Scanner::Peek() const {
    return PeekImpl(0);
}

char Scanner::PeekNext() const {
    return PeekImpl(1);
}

void Scanner::ScanString() {
    while (Peek() != '"' && !IsAtEnd()) {
        if (Peek() == '\n') {
            ++line_;
        }
        Advance();
    }

    if (IsAtEnd()) {
        lox_.Error(line_, "Unterminated string.");
        return;
    }

    assert(Peek() == '"');  // The closing "
    Advance();

    // Trim quotes
    std::string literal = source_.substr(start_ + 1, current_ - start_ - 2);
    AddLiteralToken(tokens::Type::kString, std::move(literal));
}

void Scanner::ScanNumber() {
    while (std::isdigit(Peek()) != 0) {
        Advance();
    }

    // Look for a fractional part
    if (Peek() == '.' && std::isdigit(PeekNext()) != 0) {
        Advance();
        while (std::isdigit(Peek()) != 0) {
            Advance();
        }
    }
    AddLiteralToken(tokens::Type::kNumber, source_.substr(start_, current_ - start_));
}

void Scanner::ScanIdentifier() {
    while (std::isalnum(Peek()) != 0 || Peek() == '_') {
        Advance();
    }

    std::string text = source_.substr(start_, current_ - start_);
    if (kKeywords.contains(text)) {
        AddNonLiteralToken(kKeywords.at(text));
    } else {
        AddNonLiteralToken(tokens::Type::kIdentifier);
    }
}

}  // namespace lox