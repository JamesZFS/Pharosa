cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -Wall -MMD -std=c++14")

aux_source_directory(. SRCS)
aux_source_directory(alg SRCS)
aux_source_directory(camera SRCS)
aux_source_directory(core SRCS)
aux_source_directory(geometric SRCS)
aux_source_directory(scene SRCS)
aux_source_directory(utils SRCS)
aux_source_directory(utils/solvers SRCS)

message("in 'src/', found sources:")
foreach(src_file ${SRCS})
    message("\t" ${src_file})
endforeach()
message("")

add_library(PharosaLib ${SRCS})
