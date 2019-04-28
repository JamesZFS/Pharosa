cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -Wall -MMD")

aux_source_directory(. DIR_SRCS)

message("in 'src/scene/', found sources:")
foreach(src_file ${DIR_SRCS})
    message("\t" ${src_file})
endforeach()
message("")

add_library(scene ${DIR_SRCS})
