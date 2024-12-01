add_executable(d01p02)

target_sources(d01p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/d01p02.m.cpp
)

target_link_libraries(d01p02
    PRIVATE
        fmt::fmt
        project-options
)
