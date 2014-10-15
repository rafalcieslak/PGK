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

// Include GLM
///#include <glm/glm.hpp>
//using namespace glm;

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

GLfloat g_vertex_buffer_data2[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f,  1.0f,
	-1.0f, -1.0f,
	1.0f,  1.0f,
	-1.0f,  1.0f,
};

GLfloat g_card_centers[] = {
	0.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  0.0f,
};

GLfloat card_buffer_vertices[1024];
unsigned int card_buffer_vertices_no = 0;
GLfloat card_buffer_colors[1024];
unsigned int card_buffer_colors_no = 0;
GLfloat card_buffer_centers[1024];
unsigned int card_buffer_centers_no = 0;

void clear_card_buffers(){
	card_buffer_vertices_no = 0;
	card_buffer_colors_no = 0;
	card_buffer_centers_no = 0;
}

void add_card_triangle( std::pair<float, float> pos){
	for(int i = 0; i < 12; i++)
		card_buffer_vertices[card_buffer_vertices_no*2 + i] = card_vertices[i];
	for(int i = 0; i < 18; i++)
		card_buffer_colors[card_buffer_colors_no*3 + i] = card_colors[i];
	for(int i = 0; i < 6; i++){
		card_buffer_centers[card_buffer_centers_no*2 + i*2] = pos.first;
		card_buffer_centers[card_buffer_centers_no*2 + i*2 + 1] = pos.second;
	}
	card_buffer_vertices_no += 6;
	card_buffer_colors_no += 6;
	card_buffer_centers_no += 6;
}

const unsigned int board_width = 4;
const unsigned int board_height = 4;
const unsigned int cards_no = board_height*board_width;
const float card_width = 2.0f/board_width;
const float card_height = 2.0f/board_height;

std::pair<float, float> card_xy_to_coords(unsigned int x, unsigned int y){
 	//std::cout << "card width/2 = " << card_width/2.0f << std::endl;
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

	GLint uniform_xsize, uniform_ysize;
	uniform_xsize = glGetUniformLocation(shader_program_id,"xsize");
	uniform_ysize = glGetUniformLocation(shader_program_id,"ysize");
	if(uniform_xsize == -1 || uniform_ysize == -1){
			fprintf( stderr, "xsize/ysize uniforms are missing from shader.\n" );
			glfwTerminate();
			return -1;
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	GLuint cardcenters;
	glGenBuffers(1, &cardcenters);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(card_buffer_vertices), card_buffer_vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(card_buffer_colors), card_buffer_colors, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cardcenters);
	glBufferData(GL_ARRAY_BUFFER, sizeof(card_buffer_centers), card_buffer_centers, GL_DYNAMIC_DRAW);
	do{

		clear_card_buffers();
		add_card_triangle( card_xy_to_coords(0,0) );
		//add_card_triangle( card_xy_to_coords(1,1) );
		//add_card_triangle( card_xy_to_coords(2,2) );
		/*for(int i = 0; i < card_buffer_centers_no*2; i++)
			std::cout << card_buffer_centers[i] << " ";
		std::cout << std::endl;
		*/

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(shader_program_id);
		glUniform1f(uniform_xsize, card_width*0.9);
		glUniform1f(uniform_ysize, card_height*0.9);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		// 3rd attribute buffer : centers
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, cardcenters);
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, card_buffer_vertices_no);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

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
