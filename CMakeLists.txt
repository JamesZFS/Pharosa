cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

add_executable(test.exe main.cpp)
