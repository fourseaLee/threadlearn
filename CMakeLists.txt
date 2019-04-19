cmake_minimum_required(VERSION 2.8)

project(mutilthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE "Debug")

include_directories(./)

aux_source_directory(./ DIR_SRCS)

set(link_lib pthread)

add_executable(mutilthread ${DIR_SRCS})

target_link_libraries(mutilthread ${link_lib})

