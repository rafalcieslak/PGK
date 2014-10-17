# CMake entry point
cmake_minimum_required(VERSION 2.8)
project (PGK)

find_package(OpenGL REQUIRED)


# Compile external dependencies
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
	-std=c++11
	-g
	-Wall
	-Wextra
	-Werror
)

ExternalProject_Get_Property(FreeType install_dir)

include_directories(
	external/glfw-3.0.3/include/GLFW/
	external/glm-0.9.4.0/
	external/glew-1.9.0/include/
	${install_dir}/include/freetype2
	.
)
link_directories(
	${CMAKE_BINARY_DIR}/external/freetype-2.5.3-prefix/lib
)

# zad 2
add_executable(zad2
	zad2/zad2.cpp
	common/shader.cpp
	common/shader.hpp

	zad2/FragmentShader.fragmentshader
	zad2/BoardVertexShader.vertexshader
)

add_dependencies(zad2 FreeType)

target_link_libraries(zad2
	${OPENGL_LIBRARY}
	GLFW_303
	GLEW_190
	freetype
)
set_target_properties(zad2 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zad2/")
create_target_launcher(zad2 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/zad2/")
create_default_target_launcher(zad2 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/zad2/")

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

add_custom_command(
   TARGET zad2 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/zad2${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/zad2/"
)
