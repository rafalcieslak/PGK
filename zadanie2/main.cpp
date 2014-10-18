#include <cstdlib>
#include <ctime>
#include <iostream>
#include "game.hpp"
#include "render.hpp"

// Cursor coordinates. These are also used by Render::Frame()... :(
unsigned int selection_x = 0, selection_y = 0;

int main( void )
{
	// Prepare random seed
	srand(time(0));
	// Prepare input tresholds
	bool key_pressed_up = false, key_pressed_down = false, key_pressed_left = false, key_pressed_right = false, key_pressed_space = false;
	// Reset game stats.
	Game::StartNewGame();
	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	// This is the main loop.
	do{
		// Draw a single frame.
		Render::Frame();

		// Selection movement keys
		if(!Render::IsKeyPressed(GLFW_KEY_UP))     key_pressed_up    = false;
		if(!Render::IsKeyPressed(GLFW_KEY_DOWN))   key_pressed_down  = false;
		if(!Render::IsKeyPressed(GLFW_KEY_LEFT))   key_pressed_left  = false;
		if(!Render::IsKeyPressed(GLFW_KEY_RIGHT))  key_pressed_right = false;
		if(!Render::IsKeyPressed(GLFW_KEY_SPACE))  key_pressed_space = false;
		if( Render::IsKeyPressed(GLFW_KEY_UP) &&   key_pressed_up    == false && selection_y < Game::board_height - 1) { selection_y++; key_pressed_up    = true;}
		if( Render::IsKeyPressed(GLFW_KEY_DOWN) && key_pressed_down  == false && selection_y > 0)                      { selection_y--; key_pressed_down  = true;}
		if( Render::IsKeyPressed(GLFW_KEY_LEFT) && key_pressed_left  == false && selection_x > 0)                      { selection_x--; key_pressed_left  = true;}
		if( Render::IsKeyPressed(GLFW_KEY_RIGHT) &&key_pressed_right == false && selection_x < Game::board_width - 1)  { selection_x++; key_pressed_right = true;}
		if( Render::IsKeyPressed(GLFW_KEY_SPACE) &&key_pressed_space == false){
			key_pressed_space = true;
			if(Game::game_state == Game::GAME_STATE_READY_TO_START ||
		       Game::game_state == Game::GAME_STATE_IN_PROGRESS      ){
					Game::TestCard(selection_y*Game::board_width + selection_x);
			}else if(Game::game_state == Game::GAME_STATE_FINISHED){
					Game::StartNewGame();
			}
		}

		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

	// Delete buffers, remove the window.
	Render::CleanUp();

	return 0;
}
