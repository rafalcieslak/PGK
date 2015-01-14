include(ExternalProject)

### GLFW + GLEW ###

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW glfw3)
find_package(GLEW)

option(ForceNoLibs "Forces downloading/compiling all external libraries. Useful to ensure compatibility." OFF)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

if(GLFW_FOUND AND GLEW_FOUND AND (NOT ForceNoLibs))
	message("-- GLFW 3 and GLEW found.")
	set(GLFW_GLEW_INCLUDE_DIRS ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} PARENT_SCOPE)
	set(GLFW_GLEW_LIBRARIES ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} PARENT_SCOPE)
	set(GLFW_GLEW_LIBRARY_DIRS ${GLFW_LIBDIR} PARENT_SCOPE)


else(GLFW_FOUND AND GLEW_FOUND AND (NOT ForceNoLibs))
	message("!!! GLFW 3 and/or GLEW development headers NOT FOUND!")
	message("!!! GLFW 3 and/or GLEW will be compiled automatically during build process!!!")

	add_subdirectory (glfw-3.0.3)

	include_directories( glfw-3.0.3/include/ glew-1.9.0/include )
	set(GLEW_SOURCE
		glew-1.9.0/src/glew.c
	)

	add_library( GLEW STATIC
		${GLEW_SOURCE}
		${GLFW_INCLUDE_DIRS}
	)

	target_link_libraries(GLEW
		${OPENGL_LIBRARY}
		${EXTRA_LIBS}
	)

	set(GLFW_GLEW_INCLUDE_DIRS external/glfw-3.0.3/include/ external/glew-1.9.0/include/ PARENT_SCOPE)
	set(GLFW_GLEW_LIBRARIES glfw3 GLEW Xxf86vm pthread Xi ${GLFW_LIBRARIES} PARENT_SCOPE)
	set(GLFW_GLEW_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/external/glfw-3.0.3/src/ ${CMAKE_BINARY_DIR}/external/ PARENT_SCOPE)

	set(GLFW_DEPS "GLFW GLEW GLFW_303 " PARENT_SCOPE)

endif(GLFW_FOUND AND GLEW_FOUND AND (NOT ForceNoLibs))

## GLM ##
find_path(GLM_INCLUDE_DIRS "glm/glm.hpp"
	HINTS
	"${GLM_DIR}/include"
	"${CMAKE_INSTALL_PREFIX}/include"
)

include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(
	GLM
	DEFAULT_MSG
	GLM_INCLUDE_DIRS
)

if(GLM_FOUND AND (NOT ForceNoLibs))
	message("-- GLM found.")
	set(GLM_INCLUDE_DIRS "${GLM_INCLUDE_DIR}" PARENT_SCOPE)
else(GLM_FOUND AND (NOT ForceNoLibs))
	message("!!! GLM development headers NOT FOUND!")
	message("!!! GLM will be downloaded automatically during build process!!!")

	ExternalProject_Add(GLM
		PREFIX GLM
		GIT_REPOSITORY https://github.com/g-truc/glm.git
		GIT_TAG 0.9.5
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
	    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/glm ${CMAKE_BINARY_DIR}/install/glm
		INSTALL_DIR ${CMAKE_BINARY_DIR}/install
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
	)
	ExternalProject_Get_Property(GLM install_dir)
	set(GLM_INCLUDE_DIRS "${install_dir}/" PARENT_SCOPE)
	set(GLM_DEPS "GLM " PARENT_SCOPE)
endif(GLM_FOUND AND (NOT ForceNoLibs))

## FREETYPE ##

find_package(Freetype QUIET)

if(FREETYPE_FOUND AND (NOT ForceNoLibs))
	message("-- Freetype found.")
	set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_ft2build}" PARENT_SCOPE)
	set(FREETYPE_LIBRARIES "${FREETYPE_LIBRARIES}" PARENT_SCOPE)

else(FREETYPE_FOUND AND (NOT ForceNoLibs))

	message("!!! Freetype development headers NOT FOUND!")
	message("!!! Freetype will be downloaded and compiled automatically during build process!!!")

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
	set(FREETYPE_DEPS "Freetype " PARENT_SCOPE)

endif(FREETYPE_FOUND AND (NOT ForceNoLibs))
