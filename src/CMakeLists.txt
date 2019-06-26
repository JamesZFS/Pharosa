cmake_minimum_required(VERSION 2.8.4)
project(Pharosa)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp -Wall -MMD")
set(LIBS alg camera core geometric scene utils)

aux_source_directory(. SRCS)
message("in 'src/', found sources:")
foreach(src_file ${SRCS})
    message("\t" ${src_file})
endforeach()
message("")

foreach(lib ${LIBS})
    add_subdirectory(${lib})
endforeach()

add_library(PharosaLib ${SRCS})
target_link_libraries(PharosaLib ${LIBS})
