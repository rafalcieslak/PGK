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
	CardState(int m) : model(m) {}
	int model;
	bool uncovered = false;
	bool removed = false;
	mutable double animation_start = 0.0;
	enum{
		ANIM_MODE_UNCOVER = 0,
		ANIM_MODE_COVER,
		ANIM_MODE_WAIT
	} AnimModes;
	mutable int animation_mode = -1;
};
const double animation_lengts[3] = {1.0, 0.5, 0.7};

#define CARD_MODELS 4

GLfloat card_sizes[CARD_MODELS+1] = {6,6,6,6,6};

GLfloat highlight_vertices[] = {
	-1.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f,
	1.0f, -1.0f, 1.0f, -0.5f, 0.5f, -1.0f,
	-1.0f, 1.0f, -1.0f, 0.5f, -0.5f, 1.0f,
	1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 1.0f,
};
GLfloat highlight_colors[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
};

GLfloat card_vertices[CARD_MODELS+1][12] = {
	{
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f,
},{
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f,
},{
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f,
},{
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f,
},{
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f,
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
	},{
		0.9f, 0.6f, 0.0f,
		1.0f, 1.0f, 0.2f,
		0.9f, 0.6f, 0.0f,
		0.9f, 0.6f, 0.0f,
		0.9f, 0.6f, 0.0f,
		1.0f, 1.0f, 0.2f,
	},{
		0.0f, 1.0f, 0.8f,
		0.0f, 0.8f, 0.9f,
		0.0f, 1.0f, 0.8f,
		0.0f, 1.0f, 0.8f,
		0.0f, 1.0f, 0.8f,
		0.0f, 0.8f, 0.9f,
	}
};

const unsigned int board_width = 4;
const unsigned int board_height = 4;
const unsigned int cards_no = board_height*board_width;
const float card_width = 2.0f/board_width;
const float card_height = 2.0f/board_height;

unsigned int selection_x = 0, selection_y = 0;
int card_testedA = -1;
int card_testedB = -1;

double current_time = 0.0;

std::vector<CardState> cards;

std::pair<float, float> card_xy_to_coords(unsigned int x, unsigned int y){
	float xf = -1.0f + x*card_width + card_width/2.0;
	float yf = -1.0f + y*card_height + card_height/2.0;
	return std::make_pair(xf,yf);
}

void process_space_press(){
	int n = selection_y*board_width + selection_x;
	if(cards[n].removed || cards[n].uncovered || card_testedB != -1 || n == card_testedA) return;
	cards[n].animation_start = current_time;
	cards[n].animation_mode = CardState::ANIM_MODE_UNCOVER;
	if(card_testedA == -1){ // first card from a tested pair
		card_testedA = n;
	}else{ // second card from a tested pair
		card_testedB = n;
	}
}


void finalize_animation(int n){
	if(cards[n].animation_mode == CardState::ANIM_MODE_UNCOVER){
		// Finalize card uncovering
		cards[n].uncovered = true;
		cards[n].animation_mode = -1;
		if(card_testedA == n){
			// this is the first card in a pair
			// uncovered_card = n;
		}else if(card_testedB == n){
			// this is the second card
			if( cards[card_testedB].model % CARD_MODELS == cards[card_testedA].model % CARD_MODELS){
				// match!
				cards[card_testedB].removed = true;
				cards[card_testedA].removed = true;
				card_testedB = -1;
				card_testedA = -1;
			}else{
				// not matching
				cards[card_testedA].animation_mode = CardState::ANIM_MODE_WAIT;
				cards[card_testedB].animation_mode = CardState::ANIM_MODE_WAIT;
				cards[card_testedA].animation_start = current_time;
				cards[card_testedB].animation_start = current_time;
			}
		}else{
			std::cerr << "ERROR: This card was is not being tested, it shold not be uncovered!" << std::endl;
		}
	}else if(cards[n].animation_mode == CardState::ANIM_MODE_WAIT){
		cards[card_testedA].uncovered = false;
		cards[card_testedB].uncovered = false;
		cards[card_testedA].animation_mode = CardState::ANIM_MODE_COVER;
		cards[card_testedB].animation_mode = CardState::ANIM_MODE_COVER;
		cards[card_testedA].animation_start = current_time;
		cards[card_testedB].animation_start = current_time;
	}else if(cards[n].animation_mode == CardState::ANIM_MODE_COVER){
		cards[card_testedA].animation_mode = -1;
		cards[card_testedB].animation_mode = -1;
		card_testedA = -1;
		card_testedB = -1;
	}
}

int main( void )
{
	// Test whether board dimentions make sense.
	if( cards_no % 2 == 1){
		std::cout << "Cards number with these dimentions is not an even number!" << std::endl;
		return -2;
	}

	// Prepare random seed
	srand(time(0));

	// Calculate grid lines
	GLfloat  vert_lines[4*(board_width -1)];
	GLfloat horiz_lines[4*(board_height-1)];
	GLfloat lines_colors[6*std::max(board_width,board_height)];
	for(unsigned int i = 0; i < board_width; i++){
		vert_lines[4*i + 0] = -1.0f + card_width*(i+1);
		vert_lines[4*i + 1] =  1.0f;
		vert_lines[4*i + 2] = -1.0f + card_width*(i+1);
		vert_lines[4*i + 3] = -1.0f;
	}
	for(unsigned int i = 0; i < board_height; i++){
		horiz_lines[4*i + 0] =  1.0f;
		horiz_lines[4*i + 1] = -1.0f + card_height*(i+1);
		horiz_lines[4*i + 2] = -1.0f;
		horiz_lines[4*i + 3] = -1.0f + card_height*(i+1);
	}
	for(unsigned int i = 0; i < 2*std::max(board_width,board_height); i++){
		lines_colors[3*i + 0] = 1.0f;
		lines_colors[3*i + 1] = 0.0f;
		lines_colors[3*i + 2] = 0.0f;
	}

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
	GLint uniform_xscale, uniform_yscale, uniform_centerx, uniform_centery, uniform_alpha, uniform_animphase, uniform_reversed, uniform_animmode;
	uniform_xscale = glGetUniformLocation(shader_program_id,"xscale");
	uniform_yscale = glGetUniformLocation(shader_program_id,"yscale");
	uniform_centerx = glGetUniformLocation(shader_program_id,"center_x");
	uniform_centery = glGetUniformLocation(shader_program_id,"center_y");
	uniform_alpha = glGetUniformLocation(shader_program_id,"alpha");
	uniform_animphase = glGetUniformLocation(shader_program_id,"animation_phase");
	uniform_animmode = glGetUniformLocation(shader_program_id,"animation_mode");
	uniform_reversed = glGetUniformLocation(shader_program_id,"reversed");
	if(uniform_xscale == -1 || uniform_yscale == -1 || uniform_centerx == -1 || uniform_centery == -1 || uniform_alpha == -1 || uniform_animphase == -1 || uniform_reversed == -1 || uniform_animmode == -1){
			fprintf( stderr, "A uniform is missing from shader.\n" );
			glfwTerminate();
			return -1;
	}

	GLuint vertexbuffer[8], colorbuffer[8], hlvertices, hlcolors, lines_color_buffer, vert_lines_buffer, horiz_lines_buffer;
	glGenBuffers(8, vertexbuffer);
	glGenBuffers(8, colorbuffer);
	glGenBuffers(1, &hlvertices);
	glGenBuffers(1, &hlcolors);
	glGenBuffers(1, &horiz_lines_buffer);
	glGenBuffers(1, &vert_lines_buffer);
	glGenBuffers(1, &lines_color_buffer);

	// Preparing data in buffers
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
	glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lines_colors), lines_colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vert_lines_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert_lines), vert_lines, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, horiz_lines_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(horiz_lines), horiz_lines, GL_STATIC_DRAW);

	// Prepare cards
	for(unsigned int i = 0; i < cards_no/2; i++){
		cards.push_back( CardState(i) );
		cards.push_back( CardState(i) );
	}
    std::random_shuffle(cards.begin(), cards.end());

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Prepare input tresholds
	bool key_pressed_up = false, key_pressed_down = false, key_pressed_left = false, key_pressed_right = false, key_pressed_space = false;

	while(true){
		current_time = glfwGetTime();

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use the shader
		glUseProgram(shader_program_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		// Prepare uniforms for highlight
		glUniform1f(uniform_xscale, card_width/2);
		glUniform1f(uniform_yscale, card_height/2);
		glUniform1i(uniform_animmode, -1);
		// Draw highlight
		{
			glBindBuffer(GL_ARRAY_BUFFER, hlvertices);
			glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
			glBindBuffer(GL_ARRAY_BUFFER, hlcolors);
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

			std::pair<float,float> pos = card_xy_to_coords(selection_x,selection_y);
			glUniform1f(uniform_centerx, pos.first);
			glUniform1f(uniform_centery, pos.second);

			// Is the selected card removed?
			if(cards[selection_y*board_width + selection_x].removed == false) glUniform1f(uniform_alpha, 1.0f);
			else						  									  glUniform1f(uniform_alpha, 0.35f);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(highlight_vertices)/sizeof(highlight_vertices[0]) );
		}

		// Prepare uniforms for cards
		glUniform1f(uniform_xscale, card_width/2*0.9);
		glUniform1f(uniform_yscale, card_height/2*0.9);

		// Draw cards
		for(unsigned int i = 0; i < board_height; i++){
			for(unsigned int j = 0; j < board_width; j++){

				// Take a card
				int n = i*board_width + j;

				// Set card position
				std::pair<float,float> pos = card_xy_to_coords(j,i);
				glUniform1f(uniform_centerx, pos.first);
				glUniform1f(uniform_centery, pos.second);

				// Process card's current animation
				glUniform1i(uniform_animmode, cards[n].animation_mode);
				float animation_phase = 0.0;
				if(cards[n].animation_mode != -1){
					animation_phase = (current_time - cards[n].animation_start) / animation_lengts[ cards[n].animation_mode ];
					if(animation_phase >= 1.0){
						animation_phase = 1.0;
						finalize_animation(n);
					}
					glUniform1f(uniform_animphase, animation_phase);
				}

				// Determine which vertex buffer to use depending on card state/model
				int card_data_index;
				if(cards[n].uncovered ||
				  (cards[n].animation_mode == CardState::ANIM_MODE_UNCOVER && animation_phase >= 0.5) ||
				  (cards[n].animation_mode == CardState::ANIM_MODE_COVER   && animation_phase <= 0.5) )
				       card_data_index = 1 + (cards[n].model % CARD_MODELS);
				else card_data_index = 0;

				// Is the card currently reversed?
				glUniform1i(uniform_reversed, cards[n].uncovered || cards[n].animation_mode == CardState::ANIM_MODE_COVER);
				// Is the card removed?
				if(cards[n].removed == false) glUniform1f(uniform_alpha, 1.0f);
				else						  glUniform1f(uniform_alpha, 0.2f);

				// Select buffers to use
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[card_data_index]);
				glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
				glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[card_data_index]);
				glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

				// Draw the card
				glDrawArrays(GL_TRIANGLES, 0, 3*card_sizes[card_data_index]);

			}
		}

		// Draw grid lines
		{
			glUniform1f(uniform_xscale, 1.0);
			glUniform1f(uniform_yscale, 1.0);
			glUniform1f(uniform_centerx, 0.0);
			glUniform1f(uniform_centery, 0.0);
			glUniform1i(uniform_animmode, -1);
			glUniform1f(uniform_alpha, 1.0f);
			glBindBuffer(GL_ARRAY_BUFFER, vert_lines_buffer);
			glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
			glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
			glDrawArrays(GL_LINES, 0, 2*(board_width-1));
			glBindBuffer(GL_ARRAY_BUFFER, horiz_lines_buffer);
			glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
			glDrawArrays(GL_LINES, 0, 2*(board_height-1));
		}

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glUseProgram(0);

		// Swap buffers
		glfwSwapBuffers(window);
		// Check for events
		glfwPollEvents();

		// Selection movement keys
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)    key_pressed_up =  false;
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS &&    key_pressed_up == false    && selection_y < board_height - 1) { selection_y++; key_pressed_up = true;}
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)  key_pressed_down =  false;
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS &&  key_pressed_down == false  && selection_y > 0)                { selection_y--; key_pressed_down = true;}
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)  key_pressed_left =  false;
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS &&  key_pressed_left == false  && selection_x > 0)                { selection_x--; key_pressed_left = true;}
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) key_pressed_right =  false;
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && key_pressed_right == false && selection_x < board_width - 1)  { selection_x++; key_pressed_right = true;}
		if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) key_pressed_space =  false;
		if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && key_pressed_space == false){
			key_pressed_space = true;
			process_space_press();
		}

		// Check if the ESC key was pressed or the window was closed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) == 1)
			break; // This is where we exit the loop
	}

	// Cleanup
	glDeleteBuffers(8, vertexbuffer);
	glDeleteBuffers(8, colorbuffer);
	glDeleteBuffers(1, &hlvertices);
	glDeleteBuffers(1, &hlcolors);
	glDeleteBuffers(1, &horiz_lines_buffer);
	glDeleteBuffers(1, &vert_lines_buffer);
	glDeleteBuffers(1, &lines_color_buffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shader_program_id);

	// Close the window
	glfwTerminate();

	return 0;
}
