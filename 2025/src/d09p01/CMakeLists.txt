add_executable(d09p01)

target_sources(d09p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d09p01.m.cpp
)

target_link_libraries(d09p01
    PRIVATE
        fmt::fmt
        project-options
)

