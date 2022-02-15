set(SOURCES vector.c ioutils.c mymalloc.c)

set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/vector.h" "${CMAKE_SOURCE_DIR}/include/ioutils.h" "${CMAKE_SOURCE_DIR}/include/mymalloc.h")

set(LIBNAME "vector")

add_library(${LIBNAME} ${SOURCES} ${HEADER_LIST})

target_include_directories(${LIBNAME} PUBLIC ../include)
