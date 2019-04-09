cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -Wall -MMD -std=c++14")

add_subdirectory(src)
#aux_source_directory(src SRCS)
#aux_source_directory(src/alg SRCS)
#aux_source_directory(src/camera SRCS)
#aux_source_directory(src/scene SRCS)
#aux_source_directory(src/utils/solvers SRCS)
#aux_source_directory(src/utils SRCS)

add_executable(Pharosa main.cpp)

target_link_libraries(Pharosa PharosaLib)
