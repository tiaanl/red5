project(binary_to_cpp)

set(SOURCE_FILES
    src/binary_to_cpp.cpp
    )

base_add_executable(binary_to_cpp ${SOURCE_FILES})
target_link_libraries(binary_to_cpp PUBLIC spdlog::spdlog)
