cmake_minimum_required(VERSION 3.10)
project(objwriter)

set(CMAKE_CXX_STANDARD 17)


add_library(objwriter obj.cpp)


target_include_directories(objwriter PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>")
