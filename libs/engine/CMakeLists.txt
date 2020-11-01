project(engine)

set(engine_HEADER_FILES
    include/engine/engine.h
    include/engine/stage.h
    )

set(engine_SOURCE_FILES
    src/engine.cpp
    src/stage.cpp
    )

base_add_library(engine ${engine_HEADER_FILES} ${engine_SOURCE_FILES})
target_include_directories(engine PRIVATE ${engine_BINARY_DIR})
target_link_libraries(engine PUBLIC lfd)
target_link_libraries(engine PUBLIC renderer)
target_link_libraries(engine PUBLIC SDL2::SDL2main SDL2::SDL2-static)
target_link_libraries(engine PRIVATE imgui::imgui)
