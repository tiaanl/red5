macro(base_set_flags target)
    if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        # Enable warnings and multi-process compilation.
        target_compile_options(${target} PUBLIC /W3 /WX /MP)

        set_target_properties(${target} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
            )

        # Enable memory debugging.
        # target_compile_definitions(${target} PUBLIC "-D_CRTDBG_MAP_ALLOC")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        # Enable warnings.
        target_compile_options(${target} PUBLIC -Wall -Wextra -pedantic -Werror)
    endif ()
endmacro(base_set_flags)

macro(base_set_properties target)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        )
endmacro(base_set_properties)

function(base_add_library name)
    add_library(${name} ${ARGN})

    target_include_directories(${name} PUBLIC include)

    base_set_flags(${name})
    base_set_properties(${name})
endfunction(base_add_library)

function(base_add_executable name)
    add_executable(${name} ${ARGN})

    base_set_flags(${name})
    base_set_properties(${name})
endfunction(base_add_executable)
