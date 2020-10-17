cmake_minimum_required(VERSION 3.16)

project(red5)

include(${nucleus_SOURCE_DIR}/cmake/Utils.cmake)

add_subdirectory(../nucleus nucleus)
add_subdirectory(libs)
add_subdirectory(bins)
