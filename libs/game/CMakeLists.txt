project(game)

set(game_HEADER_FILES
    include/game/font.h
    include/game/game_stage.h
    include/game/props.h
    include/game/resources.h
    include/game/scene.h
    include/game/scene_delegate.h
    include/game/single_scene_stage.h
    )

set(game_SOURCE_FILES
    src/font.cpp
    src/game_stage.cpp
    src/props.cpp
    src/resources.cpp
    src/scene.cpp
    src/scene_delegate.cpp
    src/single_scene_stage.cpp
    )

base_add_library(game ${game_HEADER_FILES} ${game_SOURCE_FILES})
target_link_libraries(game PUBLIC engine)
