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
	static GLuint grid_program_id;
	// The vertex array's ID.
	static GLuint VertexArrayID;
	static GLuint gridbuffer;
	static int gridno;
	// Pixel sizes. Needed for perfect text alignment.
	static float pxsizex, pxsizey;
	static GLint uniform_camera_transform, uniform_perspective_transform, uniform_camera_transform_g;
	static GLint uniform_perspective_transform_g, uniform_light_intensity, uniform_light_angle;
	static GLint uniform_xscale, uniform_xscale_g, uniform_sphere, uniform_sphere_g;
	static GLint uniform_terrainscale;
	// Used for scroll handling
	static void ScrollCallback(GLFWwindow*, double, double);
	static bool mouse_captured;
public:
	// Prepares for rendering, sets up the window, prepares buffers.
	static int Init();
	static bool inited;
	static int WINDOW_SIZE_X, WINDOW_SIZE_Y;
	static float WINDOW_SIZE_RATIO;
	// Renders a single frame.
	static void FrameStart(float light_intensity, float light_angle, float xscale, bool sphere, float terrainscale);
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
	static glm::vec2 ProbeMousePos();
	static bool IsMouseDown();
	static GLint uniform_pos;
	// For cursor control
	static void CaptureMouse();
	static void FreeMouse();
	// Scroll callback
	static std::function<void(double)> scroll_callback;
};


#endif //RENDER_HPP
