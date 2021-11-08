cmake_minimum_required(VERSION 3.9.0)

project(cgl VERSION 0.3 LANGUAGES CXX)

set(SRCS
	src/Actor.cpp
	src/Camera.cpp
	src/Mesh.cpp
	src/Model.cpp
	src/PrimitiveShape.cpp
	src/Resource.cpp
	src/ResourceManager.cpp
	src/Scene.cpp
	src/ShaderProgram.cpp
)

add_library(cgl ${SRCS})

target_include_directories(cgl
	PUBLIC
		$<INSTALL_INTERFACE:inlcude>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inlcude>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(cgl PRIVATE -Werror -O3)

target_compile_features(cgl PRIVATE cxx_std_20)

