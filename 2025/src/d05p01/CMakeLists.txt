add_executable(d05p01)

target_sources(d05p01
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d05p01.m.cpp
)

target_link_libraries(d05p01
    PRIVATE
        Boost::headers
        fmt::fmt
        project-options
)

