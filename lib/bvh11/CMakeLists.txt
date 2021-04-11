cmake_minimum_required(VERSION 3.10)
project(bvh11)

set(CMAKE_CXX_STANDARD 17)


add_library(bvh11 bvh11.cpp bvh11.hpp)


target_include_directories(bvh11 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>")
