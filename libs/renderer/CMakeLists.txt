project(renderer)

set(renderer_HEADER_FILES
    include/renderer/color.h
    include/renderer/program_resource.h
    include/renderer/render_target_resource.h
    include/renderer/renderer.h
    include/renderer/resource_container.h
    include/renderer/sprite.h
    include/renderer/sprite_renderer.h
    include/renderer/texture_resource.h
    include/renderer/types.h
    include/renderer/uniform_data.h
    include/renderer/vertex_buffer_definition.h
    include/renderer/vertex_buffer_resource.h
    )

set(renderer_SOURCE_FILES
    src/gl_check.cpp
    src/gl_check.h
    src/program_resource.cpp
    src/render_target_resource.cpp
    src/renderer.cpp
    src/sprite.cpp
    src/sprite_renderer.cpp
    src/texture_resource.cpp
    src/uniform_data.cpp
    src/vertex_buffer_definition.cpp
    src/vertex_buffer_resource.cpp
    )

base_add_library(renderer ${renderer_HEADER_FILES} ${renderer_SOURCE_FILES})
target_link_libraries(renderer PUBLIC base)
target_link_libraries(renderer PRIVATE glad::glad)
target_link_libraries(renderer PRIVATE glm)
