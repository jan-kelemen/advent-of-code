add_executable(d07p02)

target_sources(d07p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d07p02.m.cpp
)

target_link_libraries(d07p02
    PRIVATE
        fmt::fmt
        project-options
)

