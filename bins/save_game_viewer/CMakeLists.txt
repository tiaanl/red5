project(save_game_viewer)

set(save_game_viewer_SOURCE_FILES
    src/save_game_viewer.cpp
    )

base_add_executable(save_game_viewer ${save_game_viewer_SOURCE_FILES})
target_link_libraries(save_game_viewer PRIVATE xwing)
