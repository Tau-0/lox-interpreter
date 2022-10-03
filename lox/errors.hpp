#pragma once

#include <data_structures/tokens/tokens.hpp>
#include <stdexcept>

namespace lox {

struct RuntimeError : public std::runtime_error {
    RuntimeError(const tokens::Token& token, std::string&& message);

    tokens::Token token_;
};

struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}  // namespace lox
