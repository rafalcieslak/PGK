#ifndef RENDER_HPP
#define RENDER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <functional>
#include <memory>
#include "Mesh.hpp"

typedef enum ShaderMode{
	SHADER_MODE_BASIC,
	SHADER_MODE_TOON,
	SHADER_MODE_TOON2,
	SHADER_MODE_TOON3
} ShaderMode;

class Render{
private:
	Render() = delete; // static class
	// Shader program ID.
	static GLuint current_shader;
	static GLuint basic_shader;
	static GLuint toon_shader, toon2_shader, toon3_shader;
	// The vertex array's ID.
	static GLuint VertexArrayID;
	// Pixel sizes. Needed for perfect text alignment.
	static float pxsizex, pxsizey;
	static GLint uniform_camera_transform, uniform_perspective_transform;
	static GLint uniform_color_diffuse, uniform_color_spectral, uniform_color_ambient;
	static GLint uniform_tex_spec, uniform_tex_amb, uniform_tex_diff;
	static GLint uniform_use_tex_spec, uniform_use_tex_amb, uniform_use_tex_diff;
	static GLint uniform_specular_hardness, uniform_lightpos, uniform_alpha;
	static GLint uniform_contour_scale;
	// Used for scroll handling
	static void ScrollCallback(GLFWwindow*, double, double);
public:
	// Prepares for rendering, sets up the window, prepares buffers.
	static int Init();
	static bool inited;
	// Renders a single frame.
	static void Frame(const std::vector<std::shared_ptr<Mesh>> &, glm::vec3 lightpos, float near, float far, float contour_scale);
	// Closes the window etc.
	static void CleanUp();
	// Returns current time (in secs) since the program was launched.
	static double GetTime();
	// Returns true iff the given key (GLFW_KEY_*) is currently being held down / pressed.
	static bool IsKeyPressed(int key);
	// Returns true iff the user has closed the window.
	static bool IsWindowClosed();
	// Returns mouse pointer position scaled to [-1,1].
	static glm::vec2 GetMousePos();
	static bool IsMouseDown(int button);
	static GLint uniform_pos;
	// Scroll callback
	static std::function<void(double)> scroll_callback;
	// Shader selection
	static void SetShaderMode(ShaderMode);
};


#endif //RENDER_HPP
