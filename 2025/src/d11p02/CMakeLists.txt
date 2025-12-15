add_executable(d11p02)

target_sources(d11p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d11p02.m.cpp
)

target_link_libraries(d11p02
    PRIVATE
        fmt::fmt
        project-options
)

