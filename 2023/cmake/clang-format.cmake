if (AOC_ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_EXE NAMES clang-format-17 REQUIRED)
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")

    file(GLOB_RECURSE ALL_SOURCE_FILES d*/*.cpp d*/*.h)
    
    add_custom_target(clang-format 
        COMMAND 
            ${CLANG_FORMAT_EXE} -style=file -i ${ALL_SOURCE_FILES}
    )
endif()
