cmake_minimum_required(VERSION 3.16)

project(red5)

include(cmake/Utils.cmake)

find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(libs)
add_subdirectory(bins)
