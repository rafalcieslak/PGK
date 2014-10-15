// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

#include <common/shader.hpp>

GLfloat card_vertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f,
};

GLfloat card_colors[] = {
	1.0f, 0.3f, 0.3f,
	0.7f, 0.0f, 0.0f,
	1.0f, 0.3f, 0.3f,
	1.0f, 0.3f, 0.3f,
	1.0f, 0.3f, 0.3f,
	0.7f, 0.0f, 0.0f,
};

const unsigned int board_width = 4;
const unsigned int board_height = 4;
const unsigned int cards_no = board_height*board_width;
const float card_width = 2.0f/board_width;
const float card_height = 2.0f/board_height;

std::pair<float, float> card_xy_to_coords(unsigned int x, unsigned int y){
	float xf = -1.0f + x*card_width + card_width/2.0;
	float yf = -1.0f + y*card_height + card_height/2.0;
	return std::make_pair(xf,yf);
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Memory game", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint shader_program_id = LoadShaders( "VertexShader.vertexshader", "FragmentShader.fragmentshader" );

	// Prepare uniforms of the vertex shader
	GLint uniform_xsize, uniform_ysize, uniform_centerx, uniform_centery;
	uniform_xsize = glGetUniformLocation(shader_program_id,"xsize");
	uniform_ysize = glGetUniformLocation(shader_program_id,"ysize");
	uniform_centerx = glGetUniformLocation(shader_program_id,"center_x");
	uniform_centery = glGetUniformLocation(shader_program_id,"center_y");
	if(uniform_xsize == -1 || uniform_ysize == -1 || uniform_centerx == -1 || uniform_centery == -1){
			fprintf( stderr, "A uniform is missing from shader.\n" );
			glfwTerminate();
			return -1;
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);


	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(card_vertices), card_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(card_colors), card_colors, GL_STATIC_DRAW);

	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(shader_program_id);
		glUniform1f(uniform_xsize, card_width*0.9);
		glUniform1f(uniform_ysize, card_height*0.9);

		// Card vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		// Card colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		for(int i = 0; i < board_height; i++){
			for(int j = 0; j < board_width; j++){

				std::pair<float,float> pos = card_xy_to_coords(j,i);
				glUniform1f(uniform_centerx, pos.first);
				glUniform1f(uniform_centery, pos.second);

				// Draw the card
				glDrawArrays(GL_TRIANGLES, 0, 6);

			}
		}

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glUseProgram(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shader_program_id);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
