project(xwing)

set(xwing_HEADER_FILES
    include/xwing/controllers/main_menu_scene_controller.h
    include/xwing/controllers/register_scene_controller.h
    include/xwing/pilot_data.h
    )

set(xwing_SOURCE_FILES
    src/controllers/main_menu_scene_controller.cpp
    src/controllers/register_scene_controller.cpp
    src/pilot_data.cpp
    )

base_add_library(xwing ${xwing_HEADER_FILES} ${xwing_SOURCE_FILES})
target_link_libraries(xwing PUBLIC game)
