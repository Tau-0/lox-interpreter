#pragma once

#include <data_structures/ast/expressions.hpp>

namespace lox {

class AstPrinter {
 public:
    std::string Print(const expressions::Expr& expr) const;

    template <typename Arg>
    std::string operator()(const Arg& arg) const {
        if constexpr (std::is_same_v<Arg, expressions::String>) {
            return arg.value_;
        } else if constexpr (std::is_same_v<Arg, expressions::Number>) {
            return std::to_string(arg.value_);
        } else if constexpr (std::is_same_v<Arg, expressions::Boolean>) {
            return arg.value_ ? "true" : "false";
        } else if constexpr (std::is_same_v<Arg, expressions::Nil>) {
            return "nil";
        } else if constexpr (std::is_same_v<Arg, expressions::Unary>) {
            return Parenthesize(arg.op_.GetLexeme(), *arg.expr_);
        } else if constexpr (std::is_same_v<Arg, expressions::Binary>) {
            return Parenthesize(arg.op_.GetLexeme(), *arg.left_, *arg.right_);
        } else if constexpr (std::is_same_v<Arg, expressions::Conditional>) {
            return Parenthesize("?", *arg.first_, *arg.second_, *arg.third_);
        } else if constexpr (std::is_same_v<Arg, expressions::Grouping>) {
            return Parenthesize("group", *arg.expr_);
        } else {
            throw std::runtime_error("Unexpected expression type.");
        }
    }

 private:
    template <typename T>
    std::string ParenthesizeImpl(const T& t) const {
        return " " + t.Accept(*this);
    }

    template <typename T, typename... Args>
    std::string ParenthesizeImpl(const T& t, const Args&... args) const {
        return ParenthesizeImpl(t) + ParenthesizeImpl(args...);
    }

    template <typename... Args>
    std::string Parenthesize(const std::string& name, const Args&... args) const {
        std::string result = "(" + name;
        result += ParenthesizeImpl(args...);
        result += ")";
        return result;
    }
};

}  // namespace lox
