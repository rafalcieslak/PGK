#ifndef RENDER_HPP
#define RENDER_HPP

#include <algorithm>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Node.hpp"

class Render{
private:
	Render() = delete; // static class
	// Shader program ID.
	static GLuint shader_program_id;
	// The vertex array's ID.
	static GLuint VertexArrayID;
	// Uniform IDs.
	static GLint uniform_model_transform, uniform_camera_transform, uniform_perspective_transform;
 	static GLint uniform_anim_mode, uniform_anim_phase, uniform_lightpos, uniform_diffuse, uniform_spatial, uniform_ambient;
	static GLint uniform_single_color, uniform_boring_color, uniform_light_fixrange;
	static GLint uniform_lightnumber, uniform_lightcolor, uniform_lightparrams, uniform_light_sda;
	// Pixel sizes. Needed for perfect text alignment.
	static float pxsizex, pxsizey;
	// The root node for drawing.
	static std::shared_ptr<Node> root;
	// Tree traversal.
	static void RecursivellyProcessNode(std::shared_ptr<Node> n, glm::mat4 current_transform);
	// Used for scroll handling
	static void ScrollCallback(GLFWwindow*, double, double);
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
	// Sets the root node.
	static void SetRootNode(std::shared_ptr<Node>);
	// Resets the mouse to window center and returns it's previous position, scaled to -1,1
	static glm::vec2 ProbeMouse();
	// Scroll callback
	static std::function<void(double)> scroll_callback;
};


#endif //RENDER_HPP
