cmake_minimum_required(VERSION 3.10)
project(dt)

set(CMAKE_CXX_STANDARD 17)


add_library(dt dt/delaunay.cpp  dt/edge.cpp  dt/triangle.cpp  dt/vector2.cpp )


target_include_directories(dt PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>")
