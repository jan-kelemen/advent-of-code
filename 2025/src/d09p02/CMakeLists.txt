add_executable(d09p02)

target_sources(d09p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d09p02.m.cpp
)

target_link_libraries(d09p02
    PRIVATE
        fmt::fmt
        project-options
)

