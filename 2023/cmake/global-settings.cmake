function(add_global_settings_to_target target_name)
    target_compile_features(${target_name} PUBLIC cxx_std_23)
    set_target_properties(${target_name} PROPERTIES CXX_STANDARD_REQUIRED ON)
    set_target_properties(${target_name} PROPERTIES CXX_EXTENSIONS OFF)

    if (AOC_ENABLE_CLANG_FORMAT)
        add_dependencies(${target_name} clang-format)
    endif()

    if (MSVC)
        target_compile_definitions(${target_name} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()

    target_link_libraries(${target_name} PRIVATE project-options)
endfunction()

