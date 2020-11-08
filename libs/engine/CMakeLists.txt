project(engine)

set(engine_HEADER_FILES
    include/engine/engine.h
    include/engine/renderer/color.h
    include/engine/renderer/program_resource.h
    include/engine/renderer/render_queue.h
    include/engine/renderer/render_target_resource.h
    include/engine/renderer/renderer.h
    include/engine/renderer/resource_container.h
    include/engine/renderer/texture_resource.h
    include/engine/renderer/types.h
    include/engine/renderer/uniform_data.h
    include/engine/renderer/vertex.h
    include/engine/renderer/vertex_array_resource.h
    include/engine/renderer/vertex_buffer_definition.h
    include/engine/stage.h
    include/engine/utils/render_shapes.h
    include/engine/utils/sprite.h
    include/engine/utils/sprite_renderer.h
    )

set(engine_SOURCE_FILES
    src/engine.cpp
    src/gl_check.cpp
    src/gl_check.h
    src/program_resource.cpp
    src/render_queue.cpp
    src/render_target_resource.cpp
    src/renderer.cpp
    src/stage.cpp
    src/texture_resource.cpp
    src/uniform_data.cpp
    src/utils/render_shapes.cpp
    src/utils/sprite.cpp
    src/utils/sprite_renderer.cpp
    src/vertex_array_resource.cpp
    src/vertex_buffer_definition.cpp
    )

base_add_library(engine ${engine_HEADER_FILES} ${engine_SOURCE_FILES})
target_link_libraries(engine PUBLIC lfd)
target_link_libraries(engine PUBLIC SDL2::SDL2main SDL2::SDL2-static)
target_link_libraries(engine PRIVATE glad::glad)
target_link_libraries(engine PRIVATE imgui::imgui)
