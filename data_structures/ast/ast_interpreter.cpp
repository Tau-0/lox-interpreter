#include "ast_interpreter.hpp"

namespace lox {

Value AstInterpreter::Evaluate(const expressions::Expr& expr) const {
    return expr.Accept(*this);
}

bool AstInterpreter::IsTruthy(const lox::Value& value) const {
    static constexpr auto kVisitor = [](const auto& arg) -> bool {
        using T = std::remove_cvref_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return false;
        } else if constexpr (std::is_same_v<T, bool>) {
            return arg;
        } else {
            return true;
        }
    };

    return value.Accept(kVisitor);
}

}  // namespace lox
