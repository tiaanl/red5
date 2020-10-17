cmake_minimum_required(VERSION 3.14)

project(red5)

add_subdirectory(../nucleus nucleus)
add_subdirectory(libs)

include(${nucleus_SOURCE_DIR}/cmake/Utils.cmake)

set(SOURCE_FILES
    src/main.cpp
    src/resource/anim.cpp
    src/utils/stream.cpp
    src/utils/memory.cpp
    )

nucleus_add_executable(red5 ${SOURCE_FILES})
target_compile_definitions(red5 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(red5 PRIVATE src)
target_link_libraries(red5 PRIVATE lfd)
