project(film_viewer)

find_package(SDL2 CONFIG REQUIRED)

set(film_viewer_SOURCE_FILES
    src/film_viewer.cpp
    )

base_add_executable(film_viewer ${film_viewer_SOURCE_FILES})
target_link_libraries(film_viewer PRIVATE game)
target_link_libraries(film_viewer PRIVATE SDL2::SDL2main SDL2::SDL2-static)
