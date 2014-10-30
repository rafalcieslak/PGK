
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

### GLFW ###

add_subdirectory (glfw-3.0.3)

include_directories(
	glfw-3.0.3/include/GLFW/
	glew-1.9.0/include/
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

### GLEW ###

set(GLEW_SOURCE
	glew-1.9.0/src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW_190 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_190
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)

## FREETYPE ##

find_package(Freetype QUIET)

if(FREETYPE_FOUND)
	message("--- Freetype found.")
	set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_ft2build}" PARENT_SCOPE)
	set(FREETYPE_LIBRARIES "${FREETYPE_LIBRARIES}" PARENT_SCOPE)

else(FREETYPE_FOUND)

	message("!!! Freetype development headers NOT FOUND!")
	message("!!! Freetype will be downloaded and compiled automatically during build process!!!")

	include(ExternalProject)
	ExternalProject_Add(Freetype
		PREFIX freetype
		GIT_REPOSITORY git://git.sv.nongnu.org/freetype/freetype2.git
		# using a particular commit id, just in case they mess something up
		GIT_TAG 465ab99585cb748f9de0087d369677cc9e196785
		BUILD_COMMAND ${MAKE}
		INSTALL_DIR ${CMAKE_BINARY_DIR}/install
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
	)
	ExternalProject_Get_Property(Freetype install_dir)
	set(FREETYPE_INCLUDE_DIRS "${install_dir}/include/freetype2" PARENT_SCOPE)
	set(FREETYPE_CUSTOM_LINK_DIR "${CMAKE_BINARY_DIR}/install/lib" PARENT_SCOPE)
	set(FREETYPE_LIBRARIES "freetype" PARENT_SCOPE)

endif(FREETYPE_FOUND)
