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
            return Value::StringifyDouble(arg);
        } else {
            return {};
        }
    };

    return std::visit(kVisitor, value_);
}

std::string Value::StringifyDouble(double value) {
    std::string result(std::to_string(value));
    // Deleting trailing zeroes
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.') {
        result.pop_back();
    }
    return result;
}

}  // namespace lox
