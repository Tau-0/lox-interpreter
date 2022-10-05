#pragma once

#include <data_structures/ast/ast_interpreter.hpp>
#include <data_structures/tokens/tokens.hpp>
#include <string>

namespace lox {

struct RuntimeError;

class Lox {
 public:
    Lox();
    void RunFile(const std::string& filename);
    void RunPrompt();
    void Error(int line, const std::string& message);
    void Error(const tokens::Token& token, const std::string& message);
    void RuntimeError(const RuntimeError& error);

 private:
    void Run(std::string&& source);
    void Report(int line, const std::string& where, const std::string& message);

 private:
    AstInterpreter interpreter_;
    bool had_error_ = false;
    bool had_runtime_error_ = false;
};

}  // namespace lox
