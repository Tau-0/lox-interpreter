cmake_minimum_required(VERSION 3.23)
project(lox_interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        lox_interpreter
        main.cpp
        lox.cpp
        scanner/scanner.cpp
        scanner/tokens/tokens.cpp
        scanner/tokens/type.cpp
)
