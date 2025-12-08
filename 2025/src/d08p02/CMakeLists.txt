add_executable(d08p02)

target_sources(d08p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d08p02.m.cpp
)

target_link_libraries(d08p02
    PRIVATE
        fmt::fmt
        project-options
)

