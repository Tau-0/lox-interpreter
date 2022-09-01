#include <sysexits.h>

#include <iostream>

#include "lox.hpp"

int main(int argc, char** argv) {
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
