cmake_minimum_required(VERSION 2.8.4)
project(RayTracker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

add_executable(test.exe main.cpp
        src/Render.h src/Render.cpp src/Vec.hpp src/Ray.hpp
        src/scene/Object.cpp src/scene/Object.h src/scene/Sphere.cpp src/scene/Sphere.h)
