project(lfd)

include(${nucleus_SOURCE_DIR}/cmake/Utils.cmake)

set(lfd_HEADER_FILES
    include/lfd/film.h
    include/lfd/palette.h
    include/lfd/resource_file.h
    include/lfd/resource_type.h
    )

set(lfd_SOURCE_FILES
    src/film.cpp
    src/palette.cpp
    src/resource_file.cpp
    src/resource_type.cpp
    )

nucleus_add_library(lfd ${lfd_HEADER_FILES} ${lfd_SOURCE_FILES})
target_link_libraries(lfd PUBLIC nucleus)
