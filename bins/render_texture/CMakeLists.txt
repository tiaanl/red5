project(render_texture)

set(render_texture_SOURCE_FILES
    src/render_texture.cpp
    )

base_add_executable(render_texture ${render_texture_SOURCE_FILES})
target_link_libraries(render_texture PRIVATE engine)
