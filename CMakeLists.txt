# CMake entry point
cmake_minimum_required(VERSION 2.8)
project (PGK)

find_package(OpenGL REQUIRED)

# Compile external dependencies
add_subdirectory (external)

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

include_directories(
	${GLFW_GLEW_INCLUDE_DIRS}
	external/glm-0.9.4.0/
	${FREETYPE_INCLUDE_DIRS}
)


link_directories(
	${FREETYPE_CUSTOM_LINK_DIR} # Used if freetype was compiled manually
	${GLFW_GLEW_LIBRARY_DIRS}
)

add_executable(zadanie2
	zadanie2/main.cpp
	zadanie2/fonts.cpp
	zadanie2/fonts.hpp
	zadanie2/shader.cpp
	zadanie2/shader.hpp
	zadanie2/render.cpp
	zadanie2/render.hpp
	zadanie2/game.cpp
	zadanie2/game.hpp
	zadanie2/models.hpp

	zadanie2/BoardFragmentShader.fragmentshader
	zadanie2/BoardVertexShader.vertexshader
	zadanie2/TextFragmentShader.fragmentshader
	zadanie2/TextVertexShader.vertexshader
)

# Files needed by zadanie2 on runtime.
set( RUNTIME_FILES
	zadanie2/BoardFragmentShader.fragmentshader
	zadanie2/BoardVertexShader.vertexshader
	zadanie2/TextFragmentShader.fragmentshader
	zadanie2/TextVertexShader.vertexshader
	zadanie2/Ubuntu.ttf
)

add_dependencies(zadanie2 Freetype GLFW_303 GLFW GLEW)

if   (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(OPENGL_LIBRARY ${OPENGL_LIBRARY} -lGL -lGLU -lXrandr -lXext -lX11 -lrt)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

target_link_libraries(zadanie2
	${GLFW_GLEW_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
)

# Copy the binary
add_custom_command(
	TARGET zadanie2 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/zadanie2${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/zadanie2/"
)

# Copy runtime files
foreach( file ${RUNTIME_FILES})
    add_custom_command(
	    TARGET zadanie2 POST_BUILD
	    COMMAND ${CMAKE_COMMAND}
	    ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
)
endforeach( file )
