#include "parser.hpp"

namespace lox {

using expressions::ExprPtr;
using expressions::MakeExpr;
using tokens::Token;
using tokens::Type;

Parser::Parser(std::vector<tokens::Token>&& tokens) : tokens_(std::move(tokens)) {
}

ExprPtr Parser::Parse() {
    try {
        return Expression();
    } catch (...) {
        return nullptr;
    }
}

ExprPtr Parser::Expression() {
    return Equality();
}

ExprPtr Parser::Equality() {
    auto matcher = [this]() -> expressions::ExprPtr {
        return Comparison();
    };
    return ParseExpr(std::move(matcher), Type::kEqualEqual, Type::kBangEqual);
}

ExprPtr Parser::Comparison() {
    auto matcher = [this]() -> expressions::ExprPtr {
        return Term();
    };
    return ParseExpr(std::move(matcher), Type::kLess, Type::kLessEqual, Type::kGreater, Type::kGreaterEqual);
}

ExprPtr Parser::Term() {
    auto matcher = [this]() -> expressions::ExprPtr {
        return Factor();
    };
    return ParseExpr(std::move(matcher), Type::kMinus, Type::kPlus);
}

ExprPtr Parser::Factor() {
    auto matcher = [this]() -> expressions::ExprPtr {
        return Unary();
    };
    return ParseExpr(std::move(matcher), Type::kStar, Type::kSlash);
}

ExprPtr Parser::Unary() {
    if (Match(Type::kBang, Type::kMinus)) {
        Token op = Previous();
        return MakeExpr<expressions::Unary>(Primary(), std::move(op));
    }
    return Primary();
}

ExprPtr Parser::Primary() {
    if (Match(Type::kFalse)) {
        return MakeExpr<expressions::Boolean>(false);
    } else if (Match(Type::kTrue)) {
        return MakeExpr<expressions::Boolean>(true);
    } else if (Match(Type::kNil)) {
        return MakeExpr<expressions::Nil>();
    } else if (Match(Type::kNumber)) {
        return MakeExpr<expressions::Number>(Previous().As<tokens::literals::Number>().GetLiteral());
    } else if (Match(Type::kString)) {
        auto literal = Previous().As<tokens::literals::String>().GetLiteral();
        return MakeExpr<expressions::String>(std::move(literal));
    } else if (Match(Type::kLeftParen)) {
        auto expr = Expression();
        Consume(Type::kRightParen, "Expected ')' after expression.");
        return MakeExpr<expressions::Grouping>(std::move(expr));
    }
    // Error
    return nullptr;
}

bool Parser::Check(Type type) const {
    if (IsAtEnd()) {
        return false;
    }
    return Peek().GetType() == type;
}

const Token& Parser::Advance() {
    if (!IsAtEnd()) {
        ++current_;
    }
    return Previous();
}

bool Parser::IsAtEnd() const {
    return Peek().GetType() == Type::kEof;
}

const Token& Parser::Peek() const {
    return tokens_[current_];
}

const Token& Parser::Previous() const {
    return tokens_[current_ - 1];
}

const Token& Parser::Consume(Type type, const std::string& message) {
    if (Check(type)) {
        return Advance();
    }
    // Error
    throw;
}

bool Parser::Match(tokens::Type type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

}  // namespace lox
