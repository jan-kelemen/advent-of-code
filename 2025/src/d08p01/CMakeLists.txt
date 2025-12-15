add_executable(d08p01)

target_sources(d08p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d08p01.m.cpp
)

target_link_libraries(d08p01
    PRIVATE
        fmt::fmt
        project-options
)

