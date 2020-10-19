
find_package(SDL2 CONFIG REQUIRED)

set(FilmViewer_SOURCE_FILES
    src/film_viewer.cpp
    src/scene.h
    src/scene.cpp
    src/props.h
    src/props.cpp
    )

base_add_executable(FilmViewer WIN32 ${FilmViewer_SOURCE_FILES})
target_link_libraries(FilmViewer PRIVATE lfd)
target_link_libraries(FilmViewer PRIVATE SDL2::SDL2main SDL2::SDL2-static)
