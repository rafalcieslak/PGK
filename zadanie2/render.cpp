#include "render.hpp"
#include <iostream>
#include "game.hpp"
#include "shader.hpp"
#include "fonts.hpp"

// All the static members of Render class.
GLuint Render::vertexbuffer[9], Render::colorbuffer[9], Render::hlvertices, Render::hlcolors;
GLuint Render::lines_color_buffer, Render::vert_lines_buffer, Render::horiz_lines_buffer;
GLint Render::uniform_xscale, Render::uniform_yscale, Render::uniform_centerx;
GLint Render::uniform_centery, Render::uniform_darkening, Render::uniform_animphase;
GLint Render::uniform_reversed, Render::uniform_animmode;
GLuint Render::VertexArrayID;
float Render::pxsizex, Render::pxsizey;
float Render::card_width, Render::card_height;
GLuint Render::board_shader_program_id;

GLFWwindow* window;

// This is where card model vertices/colors are defined.
#include "models.cpp"

// Cursor position.
extern unsigned int selection_x, selection_y;

// Time (in seconds) each animation should take to complete.
const double animation_lengts[3] = {0.8, 0.5, 0.5};

std::pair<float, float> Render::card_xy_to_coords(unsigned int x, unsigned int y){
	float xf = -1.0f + x*card_width + card_width/2.0;
	float yf = -1.0f + y*card_height + card_height/2.0;
	return std::make_pair(xf,yf);
}

double Render::GetTime(){
	return glfwGetTime();
}

bool Render::IsKeyPressed(int key){
	return glfwGetKey(window,key) == GLFW_PRESS;
}

bool Render::IsWindowClosed(){
	return (glfwWindowShouldClose(window) == 1);
}

int Render::Init(){
	// Calculate helper values.
	card_width = 2.0f/Game::board_width;
	card_height = 2.0f/Game::board_height;

	// Calculate grid lines coordinates (depending on board size)
	GLfloat  vert_lines[4*(Game::board_width -1)];
	GLfloat horiz_lines[4*(Game::board_height  )];
	GLfloat lines_colors[6*std::max(Game::board_width,Game::board_height)];
	for(unsigned int i = 0; i < Game::board_width; i++){
		vert_lines[4*i + 0] = -1.0f + card_width*(i+1);
		vert_lines[4*i + 1] =  1.0f;
		vert_lines[4*i + 2] = -1.0f + card_width*(i+1);
		vert_lines[4*i + 3] = -1.0f;
	}
	for(unsigned int i = 0; i < Game::board_height+1; i++){ // one extra line for upper margin
		horiz_lines[4*i + 0] =  1.0f;
		horiz_lines[4*i + 1] = -1.0f + card_height*(i+1);
		horiz_lines[4*i + 2] = -1.0f;
		horiz_lines[4*i + 3] = -1.0f + card_height*(i+1);
	}
	for(unsigned int i = 0; i < 2*std::max(Game::board_width,Game::board_height+1); i++){
		lines_colors[3*i + 0] = 1.0f;
		lines_colors[3*i + 1] = 0.0f;
		lines_colors[3*i + 2] = 0.0f;
	}

	// Initialise GLFW
	if( !glfwInit() ){
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Memory game", NULL, NULL);
	pxsizex = 2.0 / 1024;
	pxsizey = 2.0 / 768;
	if( window == NULL ){
		std::cerr << "Failed to open GLFW window." << std::endl;
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

	// Ensure we can capture the escape key being pressed
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Prepare main vertex array.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	board_shader_program_id = LoadShaders( "BoardVertexShader.vertexshader", "BoardFragmentShader.fragmentshader" );

	// Prepare uniforms of the vertex shader
	uniform_xscale = glGetUniformLocation(board_shader_program_id,"xscale");
	uniform_yscale = glGetUniformLocation(board_shader_program_id,"yscale");
	uniform_centerx = glGetUniformLocation(board_shader_program_id,"center_x");
	uniform_centery = glGetUniformLocation(board_shader_program_id,"center_y");
	uniform_darkening = glGetUniformLocation(board_shader_program_id,"darkening");
	uniform_animphase = glGetUniformLocation(board_shader_program_id,"animation_phase");
	uniform_animmode = glGetUniformLocation(board_shader_program_id,"animation_mode");
	uniform_reversed = glGetUniformLocation(board_shader_program_id,"reversed");
	if(uniform_xscale == -1 || uniform_yscale == -1 || uniform_centerx == -1 || uniform_centery == -1 || uniform_darkening == -1 || uniform_animphase == -1 || uniform_reversed == -1 || uniform_animmode == -1){
		std::cerr << "A uniform is missing from the shader." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Prepare data buffers
	glGenBuffers(9, vertexbuffer);
	glGenBuffers(9, colorbuffer);
	glGenBuffers(1, &hlvertices);
	glGenBuffers(1, &hlcolors);
	glGenBuffers(1, &horiz_lines_buffer);
	glGenBuffers(1, &vert_lines_buffer);
	glGenBuffers(1, &lines_color_buffer);

	// Preparing data in buffers
	for(int i = 0; i <= CARD_MODELS; i++){
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * card_sizes[i] * 6, card_vertices[i], GL_STATIC_DRAW);
	}
	for(int i = 0; i <= CARD_MODELS; i++){
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * card_sizes[i] * 9, card_colors[i], GL_STATIC_DRAW);
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

	// Enable blending for several alpha effects.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Prepare for rendering fonts.
	int res = init_font();
	if(res < 0) return res;

	return 0;
}

void Render::Frame(){
	char buffer[50];

	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT );

	// Use the board shader
	glUseProgram(board_shader_program_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Prepare uniforms for highlight
	glUniform1f(uniform_xscale, card_width/2);
	glUniform1f(uniform_yscale, card_height/2);
	glUniform1i(uniform_animmode, -1);
	{
		// Draw highlight
		glBindBuffer(GL_ARRAY_BUFFER, hlvertices);
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glBindBuffer(GL_ARRAY_BUFFER, hlcolors);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		std::pair<float,float> pos = card_xy_to_coords(selection_x,selection_y);
		glUniform1f(uniform_centerx, pos.first);
		glUniform1f(uniform_centery, pos.second);

		// Is the selected card removed?
		if(Game::cards[selection_y*Game::board_width + selection_x].removed == false) glUniform1f(uniform_darkening, 0.0f);
		else						  									              glUniform1f(uniform_darkening, 0.8f);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(highlight_vertices)/sizeof(highlight_vertices[0]) );
	}

	// This causes drawn lines to be closer to the far clipping plane, making them appear "below" animated cards.
	glDepthRange (0.1, 1.0);
	{
		// Draw grid lines
		glUniform1f(uniform_xscale, 1.0);
		glUniform1f(uniform_yscale, 1.0);
		glUniform1f(uniform_centerx, 0.0);
		glUniform1f(uniform_centery, 0.0);
		glUniform1i(uniform_animmode, -1);
		glUniform1f(uniform_darkening, 0.0f);
		glBindBuffer(GL_ARRAY_BUFFER, vert_lines_buffer);
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glDrawArrays(GL_LINES, 0, 2*(Game::board_width-1));
		glBindBuffer(GL_ARRAY_BUFFER, horiz_lines_buffer);
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glDrawArrays(GL_LINES, 0, 2*(Game::board_height ));
	}

	// This causes drawn cards to be closer to the near clipping plane, making them appear "above" grid lines when animate.
	glDepthRange (0.0, 0.9);

	// Prepare uniforms for Game::cards
	glUniform1f(uniform_xscale, card_width/2*0.9);
	glUniform1f(uniform_yscale, card_height/2*0.9);

	// Draw cards
	for(unsigned int i = 0; i < Game::board_height; i++){
		for(unsigned int j = 0; j < Game::board_width; j++){

			// Take a card
			int n = i*Game::board_width + j;

			// Set card position
			std::pair<float,float> pos = card_xy_to_coords(j,i);
			glUniform1f(uniform_centerx, pos.first);
			glUniform1f(uniform_centery, pos.second);

			// Process card's current animation
			glUniform1i(uniform_animmode, Game::cards[n].animation_mode);
			float animation_phase = 0.0;
			if(Game::cards[n].animation_mode != -1){
				animation_phase = (glfwGetTime() - Game::cards[n].animation_start)
								/ animation_lengts[ Game::cards[n].animation_mode ];
				if(animation_phase >= 1.0){
					animation_phase = 1.0;
					Game::OnCardAnimationFinished(n);
				}
				//if(animation_phase >= 0.55) animation_phase = 0.55f;
				glUniform1f(uniform_animphase, animation_phase);
			}

			// Determine which vertex buffer to use depending on card state/model
			int card_data_index;
			if(Game::cards[n].uncovered ||
			(Game::cards[n].animation_mode == Card::ANIM_MODE_UNCOVER && animation_phase >= 0.5) ||
			(Game::cards[n].animation_mode == Card::ANIM_MODE_COVER   && animation_phase <= 0.5) )
				card_data_index = 1 + (Game::cards[n].model % CARD_MODELS);
			else card_data_index = 0;

			// Is the card currently reversed?
			glUniform1i(uniform_reversed, Game::cards[n].uncovered ||
										Game::cards[n].animation_mode == Card::ANIM_MODE_COVER);
			// Is the card removed?
			if(Game::cards[n].removed == false) glUniform1f(uniform_darkening, 0.0f);
			else						  glUniform1f(uniform_darkening, 0.8f);

			// Select buffers to use
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[card_data_index]);
			glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[card_data_index]);
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

			// Draw the card
			glDrawArrays(GL_TRIANGLES, 0, 3*card_sizes[card_data_index]);

		}
	}

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);


	//Render texts. For perfect text display, it has to be pixel-aligned.
	//                                                   color       fontsize
	render_text("Use cursor keys to select a card.",//  /  |  \     /
			-1.0f + 2.0f*pxsizex, 1.0f -22*pxsizey, 1.0,0.0,0.0, 24,pxsizex, pxsizey);
	if(Game::game_state != Game::GAME_STATE_FINISHED)
		render_text("Space to select. Esc to quit." ,
			-1.0f + 2.0f*pxsizex, 1.0f -48*pxsizey, 1.0,0.0,0.0, 24,pxsizex, pxsizey);
	else
		render_text("Space to start a new game. Esc to quit." ,
			-1.0f + 2.0f*pxsizex, 1.0f -48*pxsizey, 1.0,0.0,0.0, 24,pxsizex, pxsizey);
	sprintf(buffer,"Round: %d", Game::rounds);
	render_text(buffer,
			-1.0f + 2.0f*pxsizex, 1.0f -74*pxsizey, 0.0,1.0,0.4, 24,pxsizex, pxsizey);
	sprintf(buffer,"Pairs left: %d", Game::pairs_left);
	render_text(buffer,
			-1.0f+120.0f*pxsizex, 1.0f -74*pxsizey, 1.0,0.4,0.0, 24,pxsizex, pxsizey);
	sprintf(buffer,"Time: %.1f", Game::GetGameTime());
	render_text(buffer,
			-1.0f+650.0f*pxsizex, 1.0f -48*pxsizey, 0.0,1.0,0.0, 48,pxsizex, pxsizey);

	// Render congratilations message
	if(Game::game_state == Game::GAME_STATE_FINISHED){
		render_text("Congratulations!" ,
			-1.0f + 240.0f*pxsizex, 1.0f -440*pxsizey, 0.0,1.0,0.0, 72,pxsizex, pxsizey);
	}

	glUseProgram(0);

	// Swap buffers
	glfwSwapBuffers(window);
	// Check for events
	glfwPollEvents();
}

void Render::CleanUp(){
	glDeleteBuffers(9, vertexbuffer);
	glDeleteBuffers(9, colorbuffer);
	glDeleteBuffers(1, &hlvertices);
	glDeleteBuffers(1, &hlcolors);
	glDeleteBuffers(1, &horiz_lines_buffer);
	glDeleteBuffers(1, &vert_lines_buffer);
	glDeleteBuffers(1, &lines_color_buffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(board_shader_program_id);

	// Close the window.
	glfwTerminate();
}
