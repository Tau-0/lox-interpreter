cmake_minimum_required(VERSION 3.23)
project(lox_interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)
include_directories(${PROJECT_SOURCE_DIR})

file(GLOB SOURCES_TOP ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SOURCES_NESTED
        ${PROJECT_SOURCE_DIR}/data_structures/*.cpp
        ${PROJECT_SOURCE_DIR}/parser/*.cpp
        ${PROJECT_SOURCE_DIR}/scanner/*.cpp)

set(SOURCES ${SOURCES_TOP} ${SOURCES_NESTED})

add_executable(lox_interpreter ${SOURCES})
