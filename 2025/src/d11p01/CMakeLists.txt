add_executable(d11p01)

target_sources(d11p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d11p01.m.cpp
)

target_link_libraries(d11p01
    PRIVATE
        fmt::fmt
        project-options
)

