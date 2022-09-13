#include <sysexits.h>

#include <data_structures/ast/ast_printer.hpp>
#include <iostream>
#include <lox.hpp>

int main(int argc, char** argv) {
    lox::AstPrinter().Test();
    if (argc > 2) {
        std::cerr << "Usage: lox [script]\n";
        return EX_USAGE;
    }

    lox::Lox lox;
    if (argc == 2) {
        lox.RunFile(argv[1]);
    } else {
        lox.RunPrompt();
    }
    return 0;
}
