project(engine)

set(engine_HEADER_FILES
    include/engine/engine.h
    include/engine/props.h
    include/engine/scene.h
    include/engine/scene_delegate.h
    include/engine/stage.h
    include/engine/resources.h
    )

set(engine_SOURCE_FILES
    src/engine.cpp
    src/props.cpp
    src/scene.cpp
    src/scene_delegate.cpp
    src/stage.cpp
    src/resources.cpp
    )

base_add_library(engine ${engine_HEADER_FILES} ${engine_SOURCE_FILES})
target_link_libraries(engine PUBLIC lfd)
