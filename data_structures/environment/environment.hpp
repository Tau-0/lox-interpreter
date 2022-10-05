#pragma once

#include <data_structures/ast/value.hpp>
#include <data_structures/tokens/tokens.hpp>
#include <string>
#include <unordered_map>

namespace lox {

class Environment {
 public:
    Environment() = default;
    explicit Environment(Environment* enclosing);
    void Define(const std::string& name, const Value& value);
    const Value& Get(const tokens::Token& name) const;
    void Assign(const tokens::Token& name, const Value& value);

 private:
    std::unordered_map<std::string, Value> values_;
    Environment* enclosing_ = nullptr;
};

}  // namespace lox
