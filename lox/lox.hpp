#pragma once

#include <data_structures/tokens/tokens.hpp>
#include <string>

namespace lox {

class Lox {
 public:
    void RunFile(const std::string& filename);
    void RunPrompt();
    void Error(int line, const std::string& message);
    void Error(const tokens::Token& token, const std::string& message);

 private:
    void Run(std::string&& source);
    void Report(int line, const std::string& where, const std::string& message);

 private:
    bool had_error_ = false;
};

}  // namespace lox
