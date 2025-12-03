add_executable(d03p02)

target_sources(d03p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d03p02.m.cpp
)

target_link_libraries(d03p02
    PRIVATE
        fmt::fmt
        project-options
)

