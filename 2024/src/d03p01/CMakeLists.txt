add_executable(d03p01)

target_sources(d03p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/d03p01.m.cpp
)

target_link_libraries(d03p01
    PRIVATE
        fmt::fmt
        project-options
)
