add_executable(d02p02)

target_sources(d02p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/d02p02.m.cpp
)

target_link_libraries(d02p02
    PRIVATE
        fmt::fmt
        project-options
)
