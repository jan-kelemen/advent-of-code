cmake_minimum_required(VERSION 3.4)

set(TARGET_NAME AoC-2022)
project(${TARGET_NAME} VERSION 0.1)

# Export compile_commands.json during build for integration with other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/conan.cmake)
include(cmake/clang-format.cmake)

conan_cmake_configure(
    REQUIRES
        catch2/3.1.0
    OPTIONS
    GENERATORS
        cmake_find_package
)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    SETTINGS ${settings})

find_package(Catch2 REQUIRED)

add_compile_options(-W -Wall -Wextra -pedantic -Wno-logical-op-parentheses)

set(days 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25)
foreach (d IN LISTS days)
    foreach (p RANGE 1 2)
        file(GLOB_RECURSE sources "${CMAKE_CURRENT_LIST_DIR}/d${d}/p0${p}/*.cpp")
        add_executable(d${d}p0${p} ${sources})
        add_dependencies(d${d}p0${p} clang-format)
    endforeach()
endforeach()

# target_link_libraries(
#    d01p01  
#    PRIVATE
#        Catch2::Catch2WithMain
# )

