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

add_library(cgl
	STATIC
   	${SRCS}
)

set_target_properties(cgl
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"  # STATIC
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"  # SHARED
)

target_include_directories(cgl
	PUBLIC
		$<INSTALL_INTERFACE:inlcude>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inlcude>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(cgl PRIVATE -Wall -Werror -O3)

target_compile_features(cgl PRIVATE cxx_std_20)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(Bullet REQUIRED)
find_library(SOIL2 soil2)

target_link_libraries(cgl
	PRIVATE
		glfw
		OpenGL::GL
		GLEW::GLEW
		glm
		assimp
		${BULLET_LIBRARIES}
		${SOIL2}
)

target_include_directories(cgl
	PRIVATE
		${OPENGL_INCLUDE_DIR}
		${GLEW_INCLUDE_DIR}
		${GLFW3_INCLUDE_DIR}
		${GLM_INCLUDE_DIR}
		${ASSIMP_INCLUDE_DIR}
		${BULLET_INCLUDE_DIR}
  		/usr/local/include
)

