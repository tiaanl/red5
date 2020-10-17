
find_package(SDL2 CONFIG REQUIRED)

set(FilmViewer_SOURCE_FILES
    src/film_viewer.cpp
    src/scene.h
    src/scene.cpp
    )

nucleus_add_executable(FilmViewer ${FilmViewer_SOURCE_FILES})
target_link_libraries(FilmViewer PRIVATE lfd)
target_link_libraries(FilmViewer PRIVATE SDL2::SDL2main SDL2::SDL2-static)
