#include "value.hpp"

namespace lox {

bool Value::operator==(const Value& rhs) const {
    return value_ == rhs.value_;
}

bool Value::operator!=(const Value& rhs) const {
    return value_ != rhs.value_;
}

std::string Value::Stringify() const {
    static constexpr auto kVisitor = [](const auto& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "nil";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        } else if constexpr (std::is_same_v<T, bool>) {
            return arg ? "true" : "false";
        } else if constexpr (std::is_same_v<T, double>) {
            return std::to_string(arg);
        }
        return {};
    };

    return std::visit(kVisitor, value_);
}

}  // namespace lox
