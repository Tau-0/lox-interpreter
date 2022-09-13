#pragma once

#include <data_structures/tokens/type.hpp>
#include <string>

namespace lox::tokens {

template <typename C>
class Base {
 public:
    std::string ToString() const {
        std::string buffer = "Type: " + AsString(type_);
        if (!lexeme_.empty()) {
            buffer += ", Lexeme: " + lexeme_;
        }
        return buffer + ", Line: " + std::to_string(line_);
    }

    const std::string& Lexeme() const {
        return lexeme_;
    }

 protected:
    Base(Type type, std::string&& lexeme, uint32_t line) : lexeme_(std::move(lexeme)), type_(type), line_(line) {
    }

 private:
    const C* AsHeir() const {
        return static_cast<const C*>(this);
    }

    C* AsHeir() {
        return static_cast<C*>(this);
    }

 protected:
    std::string lexeme_;
    Type type_;
    uint32_t line_;
};

}  // namespace lox::tokens
