project(engine)

set(ROBOTO_FROM_FILE ${engine_SOURCE_DIR}/resources/roboto-condensed-regular.ttf)
set(ROBOTO_TO_FILE ${engine_BINARY_DIR}/roboto-condensed-regular.cpp)
add_custom_target(compile_roboto_to_cpp
    COMMAND binary_to_cpp ${ROBOTO_FROM_FILE} ${ROBOTO_TO_FILE}
    )

set(engine_HEADER_FILES
    include/engine/engine.h
    include/engine/font.h
    include/engine/props.h
    include/engine/resources.h
    include/engine/scene.h
    include/engine/scene_delegate.h
    include/engine/single_scene_stage.h
    include/engine/stage.h
    include/engine/true_type_font.h
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
    src/stb/stb_truetype.h
    src/true_type_font.cpp
    ${ROBOTO_TO_FILE}
    )

base_add_library(engine ${engine_HEADER_FILES} ${engine_SOURCE_FILES})
target_include_directories(engine PRIVATE ${engine_BINARY_DIR})
target_link_libraries(engine PUBLIC lfd)
# add_dependencies(engine compile_roboto_to_cpp)
