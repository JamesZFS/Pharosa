cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -Wall -MMD")
add_definitions(-D CONVERT_CUBE_TO_MESHES_ON=1)
add_definitions(-D KD_TREE_ON=1)
add_definitions(-D KD_N_LEAF_MESH=5)

aux_source_directory(. DIR_SRCS)

message("in 'src/scene/', found sources:")
foreach(src_file ${DIR_SRCS})
    message("\t" ${src_file})
endforeach()
message("")

add_library(scene ${DIR_SRCS})
