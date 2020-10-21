project(xwing)

set(xwing_HEADER_FILES
    include/xwing/pilot_data.h
    include/xwing/stages/register_stage.h
    )

set(xwing_SOURCE_FILES
    src/pilot_data.cpp
    src/stages/register_stage.cpp
    )

base_add_library(xwing ${xwing_HEADER_FILES} ${xwing_SOURCE_FILES})
target_link_libraries(xwing PUBLIC engine)
