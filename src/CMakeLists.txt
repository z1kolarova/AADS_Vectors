set(SOURCES vector.c)

set(HEADERS "include/vector.h")

set(LIBNAME "vector")

include(FetchContent)
FetchContent_Declare(
  myMalloc
  GIT_REPOSITORY https://mymalloc:8zj7GZZNmWyLGgQSyzsN@git.vyuka.fai.utb.cz/ads/common/mymalloc.git
)

FetchContent_MakeAvailable(myMalloc)

add_library(${LIBNAME} ${SOURCES} ${HEADERS})
target_link_libraries(${LIBNAME} PUBLIC myMalloc)

target_include_directories(${LIBNAME} PUBLIC include)
