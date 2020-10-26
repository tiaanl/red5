project(renderer)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set(renderer_HEADER_FILES
    include/renderer/color.h
    include/renderer/dimensions.h
    include/renderer/render_target.h
    include/renderer/renderer.h
    )

set(renderer_SOURCE_FILES
    src/render_target.cpp
    src/renderer.cpp
    )

base_add_library(renderer ${renderer_HEADER_FILES} ${renderer_SOURCE_FILES})
target_link_libraries(renderer PUBLIC base)
target_link_libraries(renderer PRIVATE glad::glad)
target_link_libraries(renderer PRIVATE glm)
