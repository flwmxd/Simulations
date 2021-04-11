cmake_minimum_required(VERSION 3.10)
project(bvh)

set(CMAKE_CXX_STANDARD 17)


add_library(bvh BVHParser.cpp BoneForMotionData.cpp)


target_include_directories(bvh PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>")
