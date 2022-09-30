cmake_minimum_required(VERSION 3.23)
project(lox_interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)
include_directories(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES_NESTED
        ${PROJECT_SOURCE_DIR}/data_structures/*
        ${PROJECT_SOURCE_DIR}/lox/*
        ${PROJECT_SOURCE_DIR}/parser/*
        ${PROJECT_SOURCE_DIR}/scanner/*)

set(SOURCES main.cpp ${SOURCES_NESTED})

add_executable(lox_interpreter ${SOURCES})
