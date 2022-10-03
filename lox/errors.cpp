#include "errors.hpp"

namespace lox {

RuntimeError::RuntimeError(const tokens::Token& token, std::string&& message)
    : std::runtime_error(std::move(message)), token_(token) {
}

}  // namespace lox
