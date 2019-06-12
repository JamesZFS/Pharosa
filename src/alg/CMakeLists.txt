cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -Wall -MMD")
add_definitions(-D OMP_ON=1)
add_definitions(-D LD_ON=1)
add_definitions(-D DEBUG_DRAW_ON=0)

aux_source_directory(. DIR_SRCS)

message("in 'src/alg/', found sources:")
foreach(src_file ${DIR_SRCS})
    message("\t" ${src_file})
endforeach()
message("")

add_library(alg ${DIR_SRCS})
