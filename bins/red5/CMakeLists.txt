project(red5)

find_package(SDL2 CONFIG REQUIRED)

set(red5_SOURCE_FILES
    src/red5.cpp
    )

base_add_executable(red5 ${red5_SOURCE_FILES})
target_link_libraries(red5 PRIVATE xwing)
target_link_libraries(red5 PRIVATE SDL2::SDL2main SDL2::SDL2-static)
