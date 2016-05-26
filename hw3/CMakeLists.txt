cmake_minimum_required(VERSION 3.3)
project(WebAppHw3)

find_package(MPI REQUIRED)
include_directories(${MPI_C_INCLUDE_PATH})
include_directories(${MPI_CXX_INCLUDE_PATH})

add_executable(bubble_sort_c bubble_sort.c)
target_link_libraries(bubble_sort_c ${MPI_C_LIBRARIES})
set_target_properties(bubble_sort_c PROPERTIES C_STANDARD 99)

add_executable(bubble_sort_cpp bubble_sort.cpp)
target_link_libraries(bubble_sort_cpp ${MPI_CXX_LIBRARIES})
set_target_properties(bubble_sort_cpp PROPERTIES CXX_STANDARD 11)
