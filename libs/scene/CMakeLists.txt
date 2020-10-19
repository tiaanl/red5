project(scene)

set (scene_HEADER_FILES
    include/scene/props.h
    include/scene/scene.h
    include/scene/scene_delegate.h
    )

set(scene_SOURCE_FILES
    src/props.cpp
    src/scene.cpp
    src/scene_delegate.cpp
    )

base_add_library(scene ${scene_HEADER_FILES} ${scene_SOURCE_FILES})
target_link_libraries(scene PUBLIC lfd)
