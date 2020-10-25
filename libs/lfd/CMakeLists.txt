project(lfd)

set(lfd_HEADER_FILES
    include/lfd/animation.h
    include/lfd/film.h
    include/lfd/font.h
    include/lfd/image.h
    include/lfd/palette.h
    include/lfd/resource.h
    include/lfd/resource_file.h
    include/lfd/resource_type.h
    )

set(lfd_SOURCE_FILES
    src/animation.cpp
    src/film.cpp
    src/font.cpp
    src/image.cpp
    src/palette.cpp
    src/resource_file.cpp
    src/resource_type.cpp
    )

base_add_library(lfd ${lfd_HEADER_FILES} ${lfd_SOURCE_FILES})
target_link_libraries(lfd PUBLIC base)
