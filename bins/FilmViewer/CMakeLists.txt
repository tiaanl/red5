
find_package(SDL2 CONFIG REQUIRED)

set(FilmViewer_SOURCE_FILES
    src/film_viewer.cpp
    )

base_add_executable(FilmViewer WIN32 ${FilmViewer_SOURCE_FILES})
target_link_libraries(FilmViewer PRIVATE scene)
target_link_libraries(FilmViewer PRIVATE SDL2::SDL2main SDL2::SDL2-static)
