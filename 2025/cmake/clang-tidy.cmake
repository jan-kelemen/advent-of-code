if (CST_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy-21 clang-tidy REQUIRED)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")

    if (CST_ENABLE_COLORED_COMPILER_OUTPUT)
        set(COLORED_DIAGNOSTICS -use-color)
    endif()

    set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY_EXE};
        -format-style='file';
        -header-filter=${PROJECT_SOURCE_DIR};
        -extra-arg=-Wno-unknown-warning-option;
        ${COLORED_DIAGNOSTICS}
    )
endif()

