#include "scanner.hpp"

#include <cassert>

#include "../lox.hpp"

namespace lox {

const std::unordered_map<std::string, Token::Type> Scanner::kKeywords = {
    {"and", Token::Type::kAnd},        //
    {"class", Token::Type::kClass},    //
    {"else", Token::Type::kElse},      //
    {"false", Token::Type::kFalse},    //
    {"for", Token::Type::kFor},        //
    {"fun", Token::Type::kFun},        //
    {"if", Token::Type::kIf},          //
    {"nil", Token::Type::kNil},        //
    {"or", Token::Type::kOr},          //
    {"print", Token::Type::kPrint},    //
    {"return", Token::Type::kReturn},  //
    {"super", Token::Type::kSuper},    //
    {"this", Token::Type::kThis},      //
    {"true", Token::Type::kTrue},      //
    {"var", Token::Type::kVar},        //
    {"while", Token::Type::kWhile},    //
};

Scanner::Scanner(std::string&& source, Lox& lox) : source_(std::move(source)), lox_(lox) {
}

const std::vector<Token>& Scanner::ScanTokens() {
    while (!IsAtEnd()) {
        start_ = current_;
        ScanToken();
    }
    tokens_.emplace_back(Token::Type::kEof, "", "", line_);
    return tokens_;
}

void Scanner::ScanToken() {
    char c = Advance();
    if (c == '(') {
        AddToken(Token::Type::kLeftParen);
    } else if (c == ')') {
        AddToken(Token::Type::kRightParen);
    } else if (c == '{') {
        AddToken(Token::Type::kLeftBrace);
    } else if (c == '}') {
        AddToken(Token::Type::kRightBrace);
    } else if (c == ',') {
        AddToken(Token::Type::kComma);
    } else if (c == '.') {
        AddToken(Token::Type::kDot);
    } else if (c == '-') {
        AddToken(Token::Type::kMinus);
    } else if (c == '+') {
        AddToken(Token::Type::kPlus);
    } else if (c == ';') {
        AddToken(Token::Type::kSemicolon);
    } else if (c == '*') {
        AddToken(Token::Type::kStar);
    } else if (c == '!') {
        AddToken(Match('=') ? Token::Type::kBangEqual : Token::Type::kBang);
    } else if (c == '=') {
        AddToken(Match('=') ? Token::Type::kEqualEqual : Token::Type::kEqual);
    } else if (c == '<') {
        AddToken(Match('=') ? Token::Type::kLessEqual : Token::Type::kLess);
    } else if (c == '>') {
        AddToken(Match('=') ? Token::Type::kGreaterEqual : Token::Type::kGreater);
    } else if (c == '/') {
        if (Match('/')) {
            while (Peek() != '\n' && !IsAtEnd()) {
                Advance();
            }
        } else {
            AddToken(Token::Type::kSlash);
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

void Scanner::AddToken(Token::Type type) {
    AddToken(type, "");
}

void Scanner::AddToken(Token::Type type, std::string&& literal) {
    std::string lexeme = source_.substr(start_, current_);
    tokens_.emplace_back(type, std::move(lexeme), std::move(literal), line_);
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
    std::string value = source_.substr(start_ + 1, current_ - 1);
    AddToken(Token::Type::kString, std::move(value));
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
    AddToken(Token::Type::kNumber, source_.substr(start_, current_));
}

void Scanner::ScanIdentifier() {
    while (std::isalnum(Peek()) != 0 || Peek() == '_') {
        Advance();
    }

    std::string text = source_.substr(start_, current_);
    if (!kKeywords.contains(text)) {
        AddToken(Token::Type::kIdentifier);
    } else {
        AddToken(kKeywords.at(text));
    }
}

}  // namespace lox
