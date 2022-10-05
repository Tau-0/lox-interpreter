#include "environment.hpp"

#include <lox/errors.hpp>

namespace lox {

Environment::Environment(lox::Environment* enclosing) : enclosing_(enclosing) {
}

void Environment::Define(const std::string& name, const lox::Value& value) {
    values_[name] = value;
}

const Value& Environment::Get(const tokens::Token& name) const {
    auto it = values_.find(name.GetLexeme());
    if (it == values_.end() && enclosing_ != nullptr) {
        return enclosing_->Get(name);
    } else if (it == values_.end()) {
        throw RuntimeError(name, "Undefined variable '" + name.GetLexeme() + "'.");
    }
    return it->second;
}

void Environment::Assign(const tokens::Token& name, const lox::Value& value) {
    if (values_.contains(name.GetLexeme())) {
        values_[name.GetLexeme()] = value;
    } else if (enclosing_ != nullptr) {
        enclosing_->Assign(name, value);
    } else {
        throw RuntimeError(name, "Undefined variable '" + name.GetLexeme() + "'.");
    }
}

}  // namespace lox
