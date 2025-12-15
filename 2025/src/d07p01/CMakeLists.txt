add_executable(d07p01)

target_sources(d07p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d07p01.m.cpp
)

target_link_libraries(d07p01
    PRIVATE
        fmt::fmt
        project-options
)

