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
	-O3
	-Wall
	-Wextra
	-Werror
)

if(APPLE)
	SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework QuartzCore -framework IOKit -framework Cocoa" )
endif()

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

	if(GLFW_DEPS)
	add_dependencies(zadanie2 GLEW GLFW_303)
	endif()
	if(GLM_DEPS)
	add_dependencies(zadanie2 GLM)
	endif()
	if(FREETYPE_DEPS)
	add_dependencies(zadanie2 Freetype)
	endif()


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
		zadanie3/engine/ModelBase.cpp
		zadanie3/engine/Drawable.cpp
		zadanie3/engine/Fonts.cpp
		zadanie3/engine/Render.cpp
		zadanie3/engine/Shader.cpp
		zadanie3/engine/Positionable.cpp
		zadanie3/engine/Body.cpp
		zadanie3/engine/Text.cpp
		zadanie3/engine/SimplePhysics.cpp
		zadanie3/Ball.cpp
		zadanie3/Board.cpp
		zadanie3/Paddle.cpp
		zadanie3/Brick.cpp
		zadanie3/Background.cpp
		zadanie3/Overlay.cpp
		zadanie3/main.cpp
	)

	# Files needed by zadanie3 on runtime.
	set( ZAD3_RUNTIME_FILES
		zadanie3/engine/MainFragmentShader.fragmentshader
		zadanie3/engine/MainVertexShader.vertexshader
		zadanie3/engine/TextFragmentShader.fragmentshader
		zadanie3/engine/TextVertexShader.vertexshader
		zadanie3/engine/Ubuntu.ttf
	)

	if(GLFW_DEPS)
	add_dependencies(zadanie3 GLEW GLFW_303)
	endif()
	if(GLM_DEPS)
	add_dependencies(zadanie3 GLM)
	endif()
	if(FREETYPE_DEPS)
	add_dependencies(zadanie3 Freetype)
	endif()

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

# ZADANIE4
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie4")
	add_executable(zadanie4
		zadanie4/engine/ModelBase.cpp
		zadanie4/engine/Drawable.cpp
		zadanie4/engine/Fonts.cpp
		zadanie4/engine/Render.cpp
		zadanie4/engine/Shader.cpp
		zadanie4/engine/Node.cpp
		zadanie4/engine/Text.cpp
		zadanie4/engine/Viewpoint.cpp
		zadanie4/engine/Light.cpp
		zadanie4/engine/World.cpp
		zadanie4/Cube.cpp
		zadanie4/Bubble.cpp
		zadanie4/Player.cpp
		zadanie4/main.cpp
	)

	# Files needed by zadanie4 on runtime.
	set( ZAD4_RUNTIME_FILES
		zadanie4/engine/FragmentShaderA.fragmentshader
		zadanie4/engine/VertexShaderA.vertexshader
		zadanie4/engine/TextFragmentShader.fragmentshader
		zadanie4/engine/TextVertexShader.vertexshader
		zadanie4/engine/Ubuntu.ttf
	)

	if(GLFW_DEPS)
	add_dependencies(zadanie4 GLEW GLFW_303)
	endif()
	if(GLM_DEPS)
	add_dependencies(zadanie4 GLM)
	endif()
	if(FREETYPE_DEPS)
	add_dependencies(zadanie4 Freetype)
	endif()


	target_link_libraries(zadanie4
		${GLFW_GLEW_LIBRARIES}
		${FREETYPE_LIBRARIES}
		${OPENGL_LIBRARY}
		${CMAKE_DL_LIBS}
	)

	# Copy the binary
	add_custom_command(
		TARGET zadanie4 POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/zadanie4${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/zadanie4/"
	)

	# Copy runtime files
	foreach( file ${ZAD4_RUNTIME_FILES})
		add_custom_command(
			TARGET zadanie4 POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
	)
	endforeach( file )

endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie4")


# ZADANIE5
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie5")
	add_executable(zadanie5
		zadanie5/main.cpp
		zadanie5/Tile.cpp
		zadanie5/Render.cpp
		zadanie5/Text.cpp
		zadanie5/Fonts.cpp
		zadanie5/Shader.cpp
		zadanie5/Viewpoint.cpp
	)

	# Files needed by zadanie5 on runtime.
	set( ZAD5_RUNTIME_FILES
		zadanie5/TextFragmentShader.fragmentshader
		zadanie5/TextVertexShader.vertexshader
		zadanie5/TerrainFragmentShader.fragmentshader
		zadanie5/TerrainVertexShader.vertexshader
		zadanie5/GridFragmentShader.fragmentshader
		zadanie5/GridVertexShader.vertexshader
		zadanie5/Ubuntu.ttf
	)

	if(GLFW_DEPS)
	add_dependencies(zadanie5 GLEW GLFW_303)
	endif()
	if(GLM_DEPS)
	add_dependencies(zadanie5 GLM)
	endif()
	if(FREETYPE_DEPS)
	add_dependencies(zadanie5 Freetype)
	endif()


	target_link_libraries(zadanie5
		${GLFW_GLEW_LIBRARIES}
		${FREETYPE_LIBRARIES}
		${OPENGL_LIBRARY}
		${CMAKE_DL_LIBS}
	)

	# Copy the binary
	add_custom_command(
		TARGET zadanie5 POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/zadanie5${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/zadanie5/"
	)

	# Copy runtime files
	foreach( file ${ZAD5_RUNTIME_FILES})
		add_custom_command(
			TARGET zadanie5 POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
	)
	endforeach( file )

endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie5")


# ZADANIE6
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie6")
	add_executable(zadanie6
		zadanie6/main.cpp
		zadanie6/Text.cpp
		zadanie6/Render.cpp
		zadanie6/Shader.cpp
		zadanie6/Fonts.cpp
	)

	# Files needed by zadanie6 on runtime.
	set( ZAD6_RUNTIME_FILES
		zadanie6/BasicFragmentShader.fragmentshader
		zadanie6/BasicVertexShader.vertexshader
		zadanie6/TextFragmentShader.fragmentshader
		zadanie6/TextVertexShader.vertexshader
		zadanie6/Ubuntu.ttf
	)

	if(GLFW_DEPS)
	add_dependencies(zadanie6 GLEW GLFW_303)
	endif()
	if(GLM_DEPS)
	add_dependencies(zadanie6 GLM)
	endif()
	if(FREETYPE_DEPS)
	add_dependencies(zadanie6 Freetype)
	endif()


	target_link_libraries(zadanie6
		${GLFW_GLEW_LIBRARIES}
		${FREETYPE_LIBRARIES}
		${OPENGL_LIBRARY}
		${CMAKE_DL_LIBS}
	)

	# Copy the binary
	add_custom_command(
		TARGET zadanie6 POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/zadanie6${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/zadanie6/"
	)

	# Copy runtime files
	foreach( file ${ZAD6_RUNTIME_FILES})
		add_custom_command(
			TARGET zadanie6 POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
	)
	endforeach( file )

endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie6")
