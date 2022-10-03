#pragma once

#include <string>
#include <variant>

namespace lox {

class Value {
 public:
    Value() = default;

    template <typename T>
    explicit Value(T&& value) : value_(std::forward<T>(value)) {
    }

    template <typename T>
    const T& As() const {
        return get<T>(value_);
    }

    template <typename T>
    bool Is() const {
        return holds_alternative<T>(value_);
    }

    template <typename V>
    auto Accept(const V& visitor) const {
        return std::visit(visitor, value_);
    }

    std::string Stringify() const;
    bool operator==(const Value& rhs) const;
    bool operator!=(const Value& rhs) const;

 private:
    static std::string StringifyDouble(double value);

 private:
    std::variant<std::monostate, bool, double, std::string> value_;
};

}  // namespace lox
