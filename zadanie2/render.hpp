#ifndef RENDER_HPP
#define RENDER_HPP

#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Render{
private:
	Render() = delete; // static class
	// Card dimesions ((screenspace).
	static float card_width, card_height;
	// A helper function that converts card position to screen coordinates.
	static std::pair<float, float> card_xy_to_coords(unsigned int x, unsigned int y);
	// Shader program ID.
	static GLuint board_shader_program_id;
	// All buffer IDs.
	static GLuint vertexbuffer[9], colorbuffer[9], hlvertices, hlcolors, lines_color_buffer, vert_lines_buffer, horiz_lines_buffer;
	// Uniform IDs.
	static GLint uniform_xscale, uniform_yscale, uniform_centerx, uniform_centery, uniform_darkening, uniform_animphase, uniform_reversed, uniform_animmode;
	// The vertex array's ID.
	static GLuint VertexArrayID;
	// Pixel sizes. Needed for perfect text alignment.
	static float pxsizex, pxsizey;
public:
	// Prepares for rendering, sets up the window, prepares buffers.
	static int Init();
	// Renders a single frame.
	static void Frame();
	// Deletes buffers, closes the window.
	static void CleanUp();
	// Returns current time (in secs) since the program was launched.
	static double GetTime();
	// Returns true iff the given key (GLFW_KEY_*) is currently being held down / pressed.
	static bool IsKeyPressed(int key);
	// Returns true iff the user has closed the window.
	static bool IsWindowClosed();
};


#endif //RENDER_HPP
