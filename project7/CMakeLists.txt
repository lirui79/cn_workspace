project (avltree)

cmake_minimum_required (VERSION 3.8)

#include_directories(include)

#ADD_SUBDIRECTORY(src)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(avltree ${DIR_SRCS})

#TARGET_LINK_LIBRARIES(avltree dataStruct)
