add_executable(d04p02)

target_sources(d04p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d04p02.m.cpp
)

target_link_libraries(d04p02
    PRIVATE
        fmt::fmt
        project-options
)

