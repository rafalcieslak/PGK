#ifndef RENDER_HPP
#define RENDER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <functional>

class Render{
private:
	Render() = delete; // static class
	// Shader program ID.
	static GLuint shader_program_id;
	// The vertex array's ID.
	static GLuint VertexArrayID;
	// Pixel sizes. Needed for perfect text alignment.
	static float pxsizex, pxsizey;
	static GLint uniform_camera_transform, uniform_perspective_transform, uniform_light_intensity;
	// Used for scroll handling
	static void ScrollCallback(GLFWwindow*, double, double);
public:
	// Prepares for rendering, sets up the window, prepares buffers.
	static int Init();
	static bool inited;
	// Renders a single frame.
	static void FrameStart(float light_intensity);
	static void FrameEnd();
	// Closes the window etc.
	static void CleanUp();
	// Returns current time (in secs) since the program was launched.
	static double GetTime();
	// Returns true iff the given key (GLFW_KEY_*) is currently being held down / pressed.
	static bool IsKeyPressed(int key);
	// Returns true iff the user has closed the window.
	static bool IsWindowClosed();
	// Resets the mouse to window center and returns it's previous position, scaled to -1,1
	static glm::vec2 ProbeMouse();
	static GLint uniform_pos, uniform_xscale;
	// Scroll callback
	static std::function<void(double)> scroll_callback;
};


#endif //RENDER_HPP
