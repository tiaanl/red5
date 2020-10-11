cmake_minimum_required(VERSION 3.14)

project(red5)

find_package(SDL2 CONFIG REQUIRED)

set(SOURCE_FILES
    src/main.cpp
    src/utils/memory.cpp
    src/utils/memory.h
    src/resource/film.cpp
    src/resource/film.h
    src/resource/lfd.cpp
    src/resource/lfd.h
    )

add_executable(red5 ${SOURCE_FILES})
target_compile_definitions(red5 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(red5 PRIVATE src)
target_link_libraries(red5 PRIVATE SDL2::SDL2main SDL2::SDL2-static)
set_target_properties(red5 PROPERTIES CXX_STANDARD 17)
