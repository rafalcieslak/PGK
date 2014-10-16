// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

#include <common/shader.hpp>

class CardState{
public:
	CardState(int m) : model(m){}
	int model;
	bool uncovered = false;
	bool removed = false;
	mutable int flip_phase = false;
	mutable double flip_start = 0.0;
};

#define CARD_MODELS 2

GLfloat card_sizes[CARD_MODELS+1] = {6,6,6};

GLfloat highlight_vertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f,
};
GLfloat highlight_colors[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
};

GLfloat card_vertices[CARD_MODELS+1][12] = {
	{
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f,
},{
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f,
},{
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f,
}
};

GLfloat card_colors[CARD_MODELS+1][18] = {
	{
		1.0f, 0.2f, 0.2f,
		0.7f, 0.0f, 0.0f,
		1.0f, 0.3f, 0.3f,
		1.0f, 0.3f, 0.3f,
		1.0f, 0.3f, 0.3f,
		0.7f, 0.0f, 0.0f,
	},{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.2f, 0.7f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.2f, 0.7f,
	},{
		0.0f, 0.5f, 0.0f,
		0.0f, 0.2f, 0.2f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.2f, 0.2f,
	}
};

const unsigned int board_width = 6;
const unsigned int board_height = 5;
const unsigned int cards_no = board_height*board_width;
const float card_width = 2.0f/board_width;
const float card_height = 2.0f/board_height;

unsigned int selection_x = 0, selection_y = 0;

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
	GLint uniform_xsize, uniform_ysize, uniform_centerx, uniform_centery, uniform_alpha;
	uniform_xsize = glGetUniformLocation(shader_program_id,"xsize");
	uniform_ysize = glGetUniformLocation(shader_program_id,"ysize");
	uniform_centerx = glGetUniformLocation(shader_program_id,"center_x");
	uniform_centery = glGetUniformLocation(shader_program_id,"center_y");
	uniform_alpha = glGetUniformLocation(shader_program_id,"alpha");
	if(uniform_xsize == -1 || uniform_ysize == -1 || uniform_centerx == -1 || uniform_centery == -1 || uniform_alpha == -1){
			fprintf( stderr, "A uniform is missing from shader.\n" );
			glfwTerminate();
			return -1;
	}

	// Preparing data in buffers
	GLuint vertexbuffer[8], colorbuffer[8], hlvertices, hlcolors;
	glGenBuffers(8, vertexbuffer);
	glGenBuffers(8, colorbuffer);
	glGenBuffers(1, &hlvertices);
	glGenBuffers(1, &hlcolors);

	for(int i = 0; i <= CARD_MODELS; i++){
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * card_sizes[i] * 2, card_vertices[i], GL_STATIC_DRAW);
	}
	for(int i = 0; i <= CARD_MODELS; i++){
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * card_sizes[i] * 3, card_colors[i], GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, hlvertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(highlight_vertices), highlight_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, hlcolors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(highlight_colors), highlight_colors, GL_STATIC_DRAW);

	// Preparing cards
	std::vector<CardState> cards;
	for(int i = 0; i < cards_no/2; i++){
		cards.push_back( CardState(i) );
		cards.push_back( CardState(i) );
	}

    std::random_shuffle(cards.begin(), cards.end());
	cards[3].uncovered = true;
	cards[4].uncovered = true;
	cards[6].uncovered = true;
	cards[8].uncovered = true;

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool key_pressed_up = false, key_pressed_down = false, key_pressed_left = false, key_pressed_right = false, key_pressed_space = false;

	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(shader_program_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		// Prepare scaling for cards
		glUniform1f(uniform_xsize, card_width*0.9);
		glUniform1f(uniform_ysize, card_height*0.9);

		glUniform1f(uniform_alpha, 1.0f);

		// Draw cards
		for(int i = 0; i < board_height; i++){
			for(int j = 0; j < board_width; j++){


				// Take a card
				const CardState& card = cards[i*board_width + j];

				std::pair<float,float> pos = card_xy_to_coords(j,i);
				glUniform1f(uniform_centerx, pos.first);
				glUniform1f(uniform_centery, pos.second);

				// Determine which vertex buffer to use

				int card_data_index;
				if(card.uncovered)
					card_data_index = 1 + (card.model % CARD_MODELS);
				else
					card_data_index = 0;

				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[card_data_index]);
				glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

				glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[card_data_index]);
				glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

				// Draw the card
				glDrawArrays(GL_TRIANGLES, 0, 3*card_sizes[card_data_index]);

			}
		}

		// Prepare scaling for highlight
		glUniform1f(uniform_xsize, card_width);
		glUniform1f(uniform_ysize, card_height);
		// Draw highlight
		{
			glBindBuffer(GL_ARRAY_BUFFER, hlvertices);
			glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
			glBindBuffer(GL_ARRAY_BUFFER, hlcolors);
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

			std::pair<float,float> pos = card_xy_to_coords(selection_x,selection_y);
			glUniform1f(uniform_centerx, pos.first);
			glUniform1f(uniform_centery, pos.second);

			glUniform1f(uniform_alpha, 0.35f);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);


		glUseProgram(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Check if the ESC key was pressed or the window was closed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) == 1)
			break;

		// Selection movement keys
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)    key_pressed_up = false;
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS &&    key_pressed_up == false    && selection_y < board_height - 1) { selection_y++; key_pressed_up = true;}
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)  key_pressed_down = false;
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS &&  key_pressed_down == false  && selection_y > 0)                { selection_y--; key_pressed_down = true;}
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)  key_pressed_left = false;
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS &&  key_pressed_left == false  && selection_x > 0)                { selection_x--; key_pressed_left = true;}
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) key_pressed_right = false;
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && key_pressed_right == false && selection_x < board_width - 1)  { selection_x++; key_pressed_right = true;}

	}
	while(true);

	// Cleanup VBO
	glDeleteBuffers(8, vertexbuffer);
	glDeleteBuffers(8, colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shader_program_id);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
