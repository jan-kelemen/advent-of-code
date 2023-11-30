cmake_minimum_required(VERSION 3.26)

project(AoC-2023 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(AOC_ENABLE_CLANG_FORMAT "Enable clang-format in build" ON)

include(${PROJECT_SOURCE_DIR}/cmake/clang-format.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/global-settings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/project-options.cmake)

set(days 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25)
foreach (d IN LISTS days)
    foreach (p RANGE 1 2)
        file(GLOB_RECURSE sources "${CMAKE_CURRENT_LIST_DIR}/d${d}/p0${p}/*.cpp")
        add_executable(d${d}p0${p} ${sources})
        add_global_settings_to_target(d${d}p0${p})
    endforeach()
endforeach()

