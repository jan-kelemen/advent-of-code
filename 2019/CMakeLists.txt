cmake_minimum_required(VERSION 3.4)

set(TARGET_NAME AOC2019)

project(${TARGET_NAME} VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/clang-format.cmake)
include(cmake/clang-tidy.cmake)
include(cmake/global-tools.cmake)

add_executable(${TARGET_NAME} main.cpp)

global_tools(${TARGET_NAME})

