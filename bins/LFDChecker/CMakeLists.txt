set(LFDChecker_SOURCE_FILES
    src/main.cpp
    )

nucleus_add_executable(LFDChecker ${LFDChecker_SOURCE_FILES})
target_compile_definitions(LFDChecker PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(LFDChecker PRIVATE src)
target_link_libraries(LFDChecker PRIVATE lfd)
