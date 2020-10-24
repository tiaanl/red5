project(lfd_checker)

set(lfd_checker_SOURCE_FILES
    src/lfd_checker.cpp
    )

base_add_executable(lfd_checker ${lfd_checker_SOURCE_FILES})
target_compile_definitions(lfd_checker PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(lfd_checker PRIVATE src)
target_link_libraries(lfd_checker PRIVATE lfd)
