#pragma once

#include <data_structures/ast/expressions.hpp>
#include <data_structures/tokens/tokens.hpp>
#include <lox/errors.hpp>
#include <vector>

namespace lox {

class Lox;

template <typename T>
concept IsTokenType = std::is_same_v<T, tokens::Type>;

class Parser {
 public:
    Parser(std::vector<tokens::Token>&& tokens, Lox& lox);
    expressions::ExprPtr Parse();

 private:
    expressions::ExprPtr Expression();
    expressions::ExprPtr Comma();
    expressions::ExprPtr Conditional();
    expressions::ExprPtr Equality();
    expressions::ExprPtr Comparison();
    expressions::ExprPtr Term();
    expressions::ExprPtr Factor();
    expressions::ExprPtr Unary();
    expressions::ExprPtr Primary();

    bool Check(tokens::Type type) const;
    const tokens::Token& Advance();
    bool IsAtEnd() const;
    const tokens::Token& Peek() const;
    const tokens::Token& Previous() const;
    const tokens::Token& Consume(tokens::Type type, const std::string& message);
    ParseError Error(const tokens::Token& token, const std::string& message);
    void Synchronize();

    bool Match(tokens::Type type);

    template <IsTokenType T, IsTokenType... Args>
    bool Match(T type, Args... types) {
        return Match(type) || Match(types...);
    }

    template <typename Sub, IsTokenType... Args>
    expressions::ExprPtr ParseExpr(Sub&& sub_expr, Args&&... types) {
        auto expr = sub_expr();
        while (Match(std::forward<Args>(types)...)) {
            auto op = Previous();
            auto right = sub_expr();
            expr = MakeExpr<expressions::Binary>(std::move(expr), std::move(right), std::move(op));
        }
        return expr;
    }

 private:
    std::vector<tokens::Token> tokens_;
    uint32_t current_ = 0;
    Lox& lox_;
};

}  // namespace lox
