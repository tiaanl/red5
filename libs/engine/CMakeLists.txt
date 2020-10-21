project(engine)

set(engine_HEADER_FILES
    include/engine/engine.h
    include/engine/font.h
    include/engine/props.h
    include/engine/resources.h
    include/engine/scene.h
    include/engine/scene_delegate.h
    include/engine/single_scene_stage.h
    include/engine/stage.h
    )

set(engine_SOURCE_FILES
    src/engine.cpp
    src/font.cpp
    src/props.cpp
    src/resources.cpp
    src/scene.cpp
    src/scene_delegate.cpp
    src/single_scene_stage.cpp
    src/stage.cpp
    )

base_add_library(engine ${engine_HEADER_FILES} ${engine_SOURCE_FILES})
target_link_libraries(engine PUBLIC lfd)
