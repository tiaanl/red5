project(base)

set(base_HEADER_FILES
    include/base/dimensions.h
    include/base/platform.h
    include/base/streams/file_input_stream.h
    include/base/streams/file_output_stream.h
    include/base/streams/input_stream.h
    include/base/streams/memory_input_stream.h
    include/base/streams/output_stream.h
    )

set(base_SOURCE_FILES
    src/dimensions.cpp
    src/streams/file_input_stream.cpp
    src/streams/file_output_stream.cpp
    src/streams/input_stream.cpp
    src/streams/memory_input_stream.cpp
    src/streams/output_stream.cpp
    )

base_add_library(base ${base_HEADER_FILES} ${base_SOURCE_FILES})
target_link_libraries(base PUBLIC spdlog::spdlog)
target_precompile_headers(base PUBLIC include/base/platform.h)

set(base_tests_TEST_FILES
    tests/dimensions_tests.cpp
    tests/tests.cpp
    )

base_add_executable(base_tests ${base_tests_TEST_FILES})
target_link_libraries(base_tests PRIVATE base)
target_link_libraries(base_tests PRIVATE Catch2::Catch2)
