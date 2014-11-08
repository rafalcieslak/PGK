#ifndef RENDER_HPP
#define RENDER_HPP

#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Render{
private:
	Render() = delete; // static class
	// Shader program ID.
	static GLuint shader_program_id;
	// The vertex array's ID.
	static GLuint VertexArrayID;
	// Uniform IDs.
	static GLint uniform_scale, uniform_center, uniform_angle;
	// Pixel sizes. Needed for perfect text alignment.
	static float pxsizex, pxsizey;
public:
	// Prepares for rendering, sets up the window, prepares buffers.
	static int Init();
	static bool inited;
	// Renders a single frame.
	static void Frame();
	// Closes the window etc.
	static void CleanUp();
	// Returns current time (in secs) since the program was launched.
	static double GetTime();
	// Returns true iff the given key (GLFW_KEY_*) is currently being held down / pressed.
	static bool IsKeyPressed(int key);
	// Returns true iff the user has closed the window.
	static bool IsWindowClosed();
};


#endif //RENDER_HPP
