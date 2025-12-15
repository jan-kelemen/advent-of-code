add_executable(d06p01)

target_sources(d06p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d06p01.m.cpp
)

target_link_libraries(d06p01
    PRIVATE
        fmt::fmt
        project-options
)

