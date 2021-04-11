cmake_minimum_required(VERSION 3.4.1)

project(FluidSimulation)

get_filename_component(LIB_SRC_DIR
                       ${CMAKE_SOURCE_DIR}/lib
                       ABSOLUTE)

get_filename_component(APP_SRC_DIR
                       ${CMAKE_SOURCE_DIR}/app/src
                       ABSOLUTE)

get_filename_component(ASSET_DIR
					  ${CMAKE_SOURCE_DIR}/Resources
                      ABSOLUTE)					   


set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS}")
 

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++17")


add_subdirectory(lib/delaunator)
add_subdirectory(lib/tinyobjloader)
add_subdirectory(lib/bvh11)
add_subdirectory(lib/stb_image)
add_subdirectory(lib/WavefrontObjWriter)



file(GLOB IMGUI_SRC
	${LIB_SRC_DIR}/imgui/imgui_draw.cpp
	${LIB_SRC_DIR}/imgui/imgui_widgets.cpp
	${LIB_SRC_DIR}/imgui/imgui.cpp
	${LIB_SRC_DIR}/imgui/imgui_impl_opengl3.cpp
)



file(GLOB SRC
	${APP_SRC_DIR}/*.h
	${APP_SRC_DIR}/*.cpp
	${APP_SRC_DIR}/engine/*.cpp
	${APP_SRC_DIR}/engine/*.h
	${APP_SRC_DIR}/model/*.cpp
	${APP_SRC_DIR}/model/*.h
	${APP_SRC_DIR}/filesystem/*.cpp
	${APP_SRC_DIR}/filesystem/*.h
	${APP_SRC_DIR}/opengl/*.cpp
	${APP_SRC_DIR}/opengl/*.h
	${APP_SRC_DIR}/libobj/*.cpp
	${APP_SRC_DIR}/libobj/*.h
	${APP_SRC_DIR}/event/*.cpp
	${APP_SRC_DIR}/event/*.h
	${APP_SRC_DIR}/tools/*.cpp
	${APP_SRC_DIR}/tools/*.h
	${APP_SRC_DIR}/ik/*.cpp
	${APP_SRC_DIR}/ik/*.h
	${APP_SRC_DIR}/editor/*.cpp
	${APP_SRC_DIR}/editor/*.h
	${APP_SRC_DIR}/mass/*.cpp
	${APP_SRC_DIR}/mass/*.h
	${APP_SRC_DIR}/fluid/*.cpp
	${APP_SRC_DIR}/fluid/*.h
)
	

add_library(ThirdPart STATIC ${IMGUI_SRC})


target_include_directories(ThirdPart PRIVATE
	${LIB_SRC_DIR}
	${LIB_SRC_DIR}/imgui
)


if (${Target} MATCHES "Windows")


set(OpenMP_CXX_FLAGS " /openmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "FFD")

link_directories(
	${LIB_SRC_DIR}/opengl/lib/${Arch}
	${LIB_SRC_DIR}/opencv/
)	

file(GLOB LIB_WINDOWS
	${LIB_SRC_DIR}/imgui/imgui_impl_glfw.cpp
) 

source_group(TREE ${APP_SRC_DIR} FILES ${SRC})
source_group(TREE ${LIB_SRC_DIR}/.. FILES ${IMGUI_SRC})


target_sources(ThirdPart PRIVATE ${LIB_WINDOWS} ${LIB_SRC_DIR})

target_include_directories(ThirdPart PUBLIC
	${LIB_SRC_DIR}/opengl/include
	${LIB_SRC_DIR}/imgui
	${LIB_SRC_DIR}/glm
)

target_include_directories(bvh11 PUBLIC
	${LIB_SRC_DIR}/glm
	${LIB_SRC_DIR}/opengl/include
	${LIB_SRC_DIR}/eigen-3.3.8
)

add_executable(FluidSimulation ${SRC} )

set_property(TARGET FluidSimulation PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${ASSET_DIR})
set_target_properties(FluidSimulation PROPERTIES COMPILE_FLAGS "/MP /wd4819 /std:c++17")


string(REPLACE "/" "\\" GLEW32_PATH ${LIB_SRC_DIR}/opengl/lib/${Arch}/glew32.dll)

string(REPLACE "/" "\\" GLEW32_OUT_PATH ${ASSET_DIR}/)

add_custom_command(TARGET FluidSimulation 
				   POST_BUILD
	COMMAND powershell Copy-Item ${GLEW32_PATH} ${GLEW32_OUT_PATH}
)

target_include_directories(FluidSimulation PRIVATE
	${APP_SRC_DIR}
	${LIB_SRC_DIR}/opengl/include
	${LIB_SRC_DIR}/imgui
	${LIB_SRC_DIR}/glm
	${LIB_SRC_DIR}/delaunator-cpp/include
	${LIB_SRC_DIR}/bvh11
	${LIB_SRC_DIR}/tinyobjloader
	${LIB_SRC_DIR}/stb_image
	${LIB_SRC_DIR}/WavefrontObjWriter
	${LIB_SRC_DIR}/opencv
)

target_link_libraries(FluidSimulation ThirdPart dt bvh11 opengl32.lib glfw3.lib glew.lib opencv_world450d.lib)

endif()
