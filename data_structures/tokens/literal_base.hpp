#pragma once

#include <data_structures/tokens/token_base.hpp>
#include <string>

namespace lox::tokens {

template <typename C, typename T>
class LiteralBase : public Base<LiteralBase<C, T>> {
 private:
    using Base = Base<LiteralBase<C, T>>;

 public:
    const T& Literal() const {
        return literal_;
    }

    std::string ToString() const {
        return Base::ToString() + ", Literal: " + AsHeir()->ToStringImpl();
    }

 protected:
    LiteralBase(Type type, std::string&& lexeme, T&& literal, uint32_t line)
        : Base(type, std::move(lexeme), line), literal_(std::move(literal)) {
    }

 private:
    const C* AsHeir() const {
        return static_cast<const C*>(this);
    }

    C* AsHeir() {
        return static_cast<C*>(this);
    }

 protected:
    T literal_;
};

}  // namespace lox::tokens
