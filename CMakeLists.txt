cmake_minimum_required(VERSION 2.8.4)
project(RayTracker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

aux_source_directory(src/ DIR_SRCS)

#add_executable(test.exe main.cpp ${DIR_SRCS})
add_executable(test.exe main.cpp
        src/Vec.hpp src/Ray.hpp
#        src/Render.h src/Render.cpp
        src/scene/Object.cpp src/scene/Object.h
        src/scene/Sphere.cpp src/scene/Sphere.h
        src/scene/Triangle.cpp src/scene/Triangle.h
        src/scene/Stage.cpp src/scene/Stage.h
        src/scene/All.h
        src/utils/Linear.hpp src/utils/Solvers.h)
