add_executable(d06p02)

target_sources(d06p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d06p02.m.cpp
)

target_link_libraries(d06p02
    PRIVATE
        fmt::fmt
        project-options
)

