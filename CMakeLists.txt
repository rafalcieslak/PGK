# CMake entry point
cmake_minimum_required(VERSION 2.8)
project (PGK)

find_package(OpenGL REQUIRED)

# Compile external dependencies
add_subdirectory (external)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	message(STATUS "Using clang.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

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
	${GLM_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
)

if   (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(OPENGL_LIBRARY ${OPENGL_LIBRARY} -lGL -lGLU -lXrandr -lXext -lX11 -lrt)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

link_directories(
	${FREETYPE_CUSTOM_LINK_DIR} # Used if freetype was compiled manually
	${GLFW_GLEW_LIBRARY_DIRS}
)

# ZADANIE 2
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie2")
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
	set( ZAD2_RUNTIME_FILES
		zadanie2/BoardFragmentShader.fragmentshader
		zadanie2/BoardVertexShader.vertexshader
		zadanie2/TextFragmentShader.fragmentshader
		zadanie2/TextVertexShader.vertexshader
		zadanie2/Ubuntu.ttf
	)

	add_dependencies(zadanie2 Freetype GLFW_303 GLFW GLEW)

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
	foreach( file ${ZAD2_RUNTIME_FILES})
	    add_custom_command(
		    TARGET zadanie2 POST_BUILD
		    COMMAND ${CMAKE_COMMAND}
		    ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
	)
	endforeach( file )
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie2")


# ZADANIE3
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie3")
	add_executable(zadanie3
		zadanie3/main.cpp
		zadanie3/Brick.cpp
		zadanie3/ModelBase.cpp
		zadanie3/Drawable.cpp
		zadanie3/fonts.cpp
		zadanie3/Render.cpp
		zadanie3/shader.cpp
		zadanie3/Positionable.cpp
		zadanie3/body.cpp
		zadanie3/simplephysics.cpp
		zadanie3/Ball.cpp
		zadanie3/Board.cpp
		zadanie3/Paddle.cpp
	)

	# Files needed by zadanie3 on runtime.
	set( ZAD3_RUNTIME_FILES
		zadanie3/MainFragmentShader.fragmentshader
		zadanie3/MainVertexShader.vertexshader
		zadanie3/TextFragmentShader.fragmentshader
		zadanie3/TextVertexShader.vertexshader
		zadanie3/Ubuntu.ttf
	)

	add_dependencies(zadanie3 Freetype GLFW_303 GLFW GLEW GLM)

	target_link_libraries(zadanie3
		${GLFW_GLEW_LIBRARIES}
		${FREETYPE_LIBRARIES}
		${OPENGL_LIBRARY}
		${CMAKE_DL_LIBS}
	)

	# Copy the binary
	add_custom_command(
		TARGET zadanie3 POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/zadanie3${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/zadanie3/"
	)

	# Copy runtime files
	foreach( file ${ZAD3_RUNTIME_FILES})
		add_custom_command(
			TARGET zadanie3 POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
	)
	endforeach( file )
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie3")
