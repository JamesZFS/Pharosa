cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -Wall -MMD")

add_subdirectory(src)

add_executable(Pharosa main.cpp)

target_link_libraries(Pharosa PharosaLib)
