#include "lox.hpp"

#include <sysexits.h>

#include <data_structures/ast/ast_printer.hpp>
#include <fstream>
#include <iostream>
#include <lox/errors.hpp>
#include <parser/parser.hpp>
#include <scanner/scanner.hpp>

namespace lox {

Lox::Lox() : interpreter_(*this) {
}

void Lox::RunFile(const std::string& filename) {
    std::ifstream file_stream(filename);
    std::string source{std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>()};
    Run(std::move(source));
    if (had_error_) {
        std::exit(EX_DATAERR);
    } else if (had_runtime_error_) {
        std::exit(EX_SOFTWARE);
    }
}

void Lox::RunPrompt() {
    while (!std::cin.eof()) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        Run(std::move(line));
        had_error_ = false;
        had_runtime_error_ = false;
    }
}

void Lox::Error(int line, const std::string& message) {
    Report(line, "", message);
}

void Lox::Error(const tokens::Token& token, const std::string& message) {
    if (token.GetType() == tokens::Type::kEof) {
        Report(token.GetLine(), " at end", message);
    } else {
        Report(token.GetLine(), " at '" + token.GetLexeme() + "'", message);
    }
}

void Lox::RuntimeError(const lox::RuntimeError& error) {
    std::cerr << error.what() << "\n[line " << error.token_.GetLine() << "]";
    had_runtime_error_ = true;
}

void Lox::Run(std::string&& source) {
    Scanner scanner(std::move(source), *this);
    Parser parser(scanner.ScanTokens(), *this);
    auto statements = parser.Parse();
    if (statements.empty() || had_error_) {
        return;
    }
    interpreter_.Interpret(statements);
}

void Lox::Report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error " << where << ": " << message << "\n";
    had_error_ = true;
}

}  // namespace lox
