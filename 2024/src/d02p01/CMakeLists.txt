add_executable(d02p01)

target_sources(d02p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/d02p01.m.cpp
)

target_link_libraries(d02p01
    PRIVATE
        fmt::fmt
        project-options
)
