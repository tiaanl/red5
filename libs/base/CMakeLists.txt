project(base)

include(cmake/Utils.cmake)

set(base_HEADER_FILES
    include/base/logging.h
    include/base/platform.h
    include/base/streams/file_input_stream.h
    include/base/streams/file_output_stream.h
    include/base/streams/input_stream.h
    include/base/streams/memory_input_stream.h
    include/base/streams/output_stream.h
    )

set(base_SOURCE_FILES
    src/streams/file_input_stream.cpp
    src/streams/file_output_stream.cpp
    src/streams/input_stream.cpp
    src/streams/memory_input_stream.cpp
    src/streams/output_stream.cpp
    )

base_add_library(base ${base_HEADER_FILES} ${base_SOURCE_FILES})
target_link_libraries(base PUBLIC spdlog::spdlog)
# target_link_libraries(base PUBLIC spdlog::spdlog spdlog::spdlog_header_only)
