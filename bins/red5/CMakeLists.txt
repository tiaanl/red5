project(red5)

set(red5_SOURCE_FILES
    src/red5.cpp
    )

base_add_executable(red5 ${red5_SOURCE_FILES})
target_link_libraries(red5 PRIVATE xwing)
