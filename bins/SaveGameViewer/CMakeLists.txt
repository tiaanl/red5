project(SaveGameViewer)

set(SaveGameViewer_SOURCE_FILES
    src/save_game_viewer.cpp
    )

base_add_executable(SaveGameViewer ${SaveGameViewer_SOURCE_FILES})
target_link_libraries(SaveGameViewer PRIVATE xwing)
