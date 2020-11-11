project(game)

set(game_HEADER_FILES
    include/game/font.h
    include/game/game_stage_state.h
    include/game/playback_controls.h
    include/game/props.h
    include/game/resources.h
    include/game/scene.h
    include/game/scene_controller.h
    include/game/scene_delegate.h
    include/game/scene_renderer.h
    include/game/scene_stage.h
    include/game/timeline.h
    )

set(game_SOURCE_FILES
    src/font.cpp
    src/game_stage_state.cpp
    src/playback_controls.cpp
    src/props.cpp
    src/resources.cpp
    src/scene.cpp
    src/scene_controller.cpp
    src/scene_delegate.cpp
    src/scene_renderer.cpp
    src/scene_stage.cpp
    src/timeline.cpp
    )

base_add_library(game ${game_HEADER_FILES} ${game_SOURCE_FILES})
target_link_libraries(game PUBLIC engine)
target_precompile_headers(game PUBLIC include/game/precompiled.h)

set(game_TEST_FILES
    tests/playback_controls_tests.cpp
    tests/timeline_tests.cpp
    )

base_add_executable(game_tests ${game_TEST_FILES})
target_link_libraries(game_tests PRIVATE game)
target_link_libraries(game_tests PRIVATE Catch2::Catch2)
