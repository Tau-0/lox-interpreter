#include "scanner.hpp"

#include <cassert>
#include <lox/lox.hpp>

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

std::vector<tokens::Token> Scanner::ScanTokens() {
    while (!IsAtEnd()) {
        start_ = current_;
        ScanToken();
    }
    tokens_.emplace_back(tokens::NonLiteral(tokens::Type::kEof, "", line_));
    return std::move(tokens_);
}

void Scanner::ScanToken() {
    char c = Advance();
    if (c == '(') {
        AddToken(tokens::Type::kLeftParen);
    } else if (c == ')') {
        AddToken(tokens::Type::kRightParen);
    } else if (c == '{') {
        AddToken(tokens::Type::kLeftBrace);
    } else if (c == '}') {
        AddToken(tokens::Type::kRightBrace);
    } else if (c == ',') {
        AddToken(tokens::Type::kComma);
    } else if (c == '.') {
        AddToken(tokens::Type::kDot);
    } else if (c == '-') {
        AddToken(tokens::Type::kMinus);
    } else if (c == '+') {
        AddToken(tokens::Type::kPlus);
    } else if (c == ';') {
        AddToken(tokens::Type::kSemicolon);
    } else if (c == '*') {
        AddToken(tokens::Type::kStar);
    } else if (c == '!') {
        AddToken(Match('=') ? tokens::Type::kBangEqual : tokens::Type::kBang);
    } else if (c == '=') {
        AddToken(Match('=') ? tokens::Type::kEqualEqual : tokens::Type::kEqual);
    } else if (c == '<') {
        AddToken(Match('=') ? tokens::Type::kLessEqual : tokens::Type::kLess);
    } else if (c == '>') {
        AddToken(Match('=') ? tokens::Type::kGreaterEqual : tokens::Type::kGreater);
    } else if (c == '?') {
        AddToken(tokens::Type::kQuestion);
    } else if (c == ':') {
        AddToken(tokens::Type::kColon);
    } else if (c == '/' && Match('/')) {
        SkipLineComment();
    } else if (c == '/' && Match('*')) {
        SkipBlockComment();
    } else if (c == '/') {
        AddToken(tokens::Type::kSlash);
    } else if (c == ' ' || c == '\r' || c == '\t') {
        // Skip
        return;
    } else if (c == '\n') {
        ++line_;
    } else if (c == '"') {
        ScanString();
    } else if (IsDigit(c)) {
        ScanNumber();
    } else if (IsAlpha(c)) {
        ScanIdentifierOrKeyword();
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

void Scanner::AddToken(tokens::Type type, std::optional<std::string>&& literal) {
    std::string lexeme = source_.substr(start_, current_ - start_);
    if (type == tokens::Type::kNumber) {
        assert(literal.has_value());
        tokens_.emplace_back(tokens::Number(type, std::move(lexeme), std::stod(*literal), line_));
    } else if (type == tokens::Type::kString) {
        assert(literal.has_value());
        tokens_.emplace_back(tokens::String(type, std::move(lexeme), std::move(*literal), line_));
    } else {
        tokens_.emplace_back(tokens::NonLiteral(type, std::move(lexeme), line_));
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
    AddToken(tokens::Type::kString, std::move(literal));
}

void Scanner::ScanNumber() {
    while (IsDigit(Peek())) {
        Advance();
    }

    // Look for a fractional part
    if (Peek() == '.' && IsDigit(PeekNext())) {
        Advance();
        while (IsDigit(Peek())) {
            Advance();
        }
    }
    AddToken(tokens::Type::kNumber, source_.substr(start_, current_ - start_));
}

void Scanner::ScanIdentifierOrKeyword() {
    while (IsAlphaNumeric(Peek())) {
        Advance();
    }

    std::string text = source_.substr(start_, current_ - start_);
    if (kKeywords.contains(text)) {
        AddToken(kKeywords.at(text));
    } else {
        AddToken(tokens::Type::kIdentifier);
    }
}

void Scanner::SkipLineComment() {
    while (Peek() != '\n' && !IsAtEnd()) {
        Advance();
    }
}

void Scanner::SkipBlockComment() {
    size_t nesting = 1;
    while (nesting > 0) {
        if (Peek() == '\0') {
            lox_.Error(line_, "Unterminated block comment.");
            return;
        }

        if (Peek() == '/' && PeekNext() == '*') {
            ++nesting;
            Advance();
            Advance();
            continue;
        } else if (Peek() == '*' && PeekNext() == '/') {
            --nesting;
            Advance();
            Advance();
            continue;
        }

        Advance();
    }
}

bool Scanner::IsDigit(unsigned char c) {
    return std::isdigit(c) != 0;
}

bool Scanner::IsAlpha(unsigned char c) {
    return std::isalpha(c) != 0 || c == '_';
}

bool Scanner::IsAlphaNumeric(unsigned char c) {
    return IsAlpha(c) || IsDigit(c);
}

}  // namespace lox
