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
    auto expr = Comparison();
    while (Match(Type::kEqualEqual, Type::kBangEqual)) {
        Token op = Previous();
        auto right = Comparison();
        expr = MakeExpr<expressions::Binary>(std::move(expr), std::move(right), std::move(op));
    }
    return expr;
}

ExprPtr Parser::Comparison() {
    auto expr = Term();
    while (Match(Type::kLess, Type::kLessEqual, Type::kGreater, Type::kGreaterEqual)) {
        Token op = Previous();
        auto right = Term();
        expr = MakeExpr<expressions::Binary>(std::move(expr), std::move(right), std::move(op));
    }
    return expr;
}

ExprPtr Parser::Term() {
    auto expr = Factor();
    while (Match(Type::kMinus, Type::kPlus)) {
        Token op = Previous();
        auto right = Factor();
        expr = MakeExpr<expressions::Binary>(std::move(expr), std::move(right), std::move(op));
    }
    return expr;
}

ExprPtr Parser::Factor() {
    auto expr = Unary();
    while (Match(Type::kStar, Type::kSlash)) {
        Token op = Previous();
        auto right = Unary();
        expr = MakeExpr<expressions::Binary>(std::move(expr), std::move(right), std::move(op));
    }
    return expr;
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
