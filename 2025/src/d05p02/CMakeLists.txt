add_executable(d05p02)

target_sources(d05p02
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/d05p02.m.cpp
)

target_link_libraries(d05p02
    PRIVATE
        Boost::headers
        fmt::fmt
        project-options
)

