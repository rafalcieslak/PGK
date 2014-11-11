#include <glm/glm.hpp>
#include "Board.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Background.hpp"
#include "Paddle.hpp"
#include "Overlay.hpp"
#include "../engine/Text.hpp"
#include "../engine/Render.hpp"
#include "../engine/SimplePhysics.hpp"
#include <ctime>
#include <iostream>
#include <cstdlib>

#define PADDLE_SIZE 0.12f
#define BRICK_HEIGHT 0.09
#define BRICK_WIDTH 2.0*BRICK_HEIGHT
#define BALL_VELOCITY 0.7f
#define PADDLE_SPEED 0.4f
#define BALLS_MAX 3
#define LEVEL_SIZE 1

typedef enum GameState{
	GAME_STATE_NONE,
	GAME_STATE_NOT_STARTED,
	GAME_STATE_IN_PROGRESS,
	GAME_STATE_WON,
	GAME_STATE_LOST
} GameState;

std::shared_ptr<Ball> ball;
std::shared_ptr<Positionable> root, level;
std::shared_ptr<Text> balls_txt, bricks_txt;
std::shared_ptr<Text> game_start_txt, game_won_txt, game_lost_txt, game_restart_txt;
std::shared_ptr<Overlay> overlay;
unsigned int balls_left, brick_count, bricks_max;
GameState game_state = GAME_STATE_NONE;

void UpdateStatusTexts();
void GameWon();

void AddBrick(int x, int y, int variant, std::shared_ptr<Positionable> parent){
	float xoffset = 0.0;
	if((y>0?y:-y)%2) xoffset = BRICK_WIDTH/2.0;
	auto brick = Brick::Create(glm::vec2(x*BRICK_WIDTH + xoffset, y*BRICK_HEIGHT),variant,BRICK_HEIGHT);
	brick->on_ball_collision.Subscribe([&](){
		brick_count--;
		if(brick_count == 0) GameWon();
		else UpdateStatusTexts();
	});
	parent->LinkChild(brick);
	brick_count++;
}


void CreateLevel(){
	if(level){
		level->DetachFromParent();
		SimplePhysics::UnRegisterSubtree(level);
	}
	level = Positionable::Create();
	brick_count = 0;
	for(int y = 0; y < LEVEL_SIZE; y++){
		for(int x = -4+y/2; x <= 4-y/2; x++){
			if(y%2!=1 || x<4-y/2)
				AddBrick(x,y,rand()%6,level);
		}
	}
	bricks_max = brick_count;
	level->SetPosRelative(glm::vec2(0.0,0.3));
	root->LinkChild(level);
	SimplePhysics::RegisterSubtree(level);
}

void SpawnNewBall(){
	if(ball){
		ball->DetachFromParent();
		SimplePhysics::UnRegisterSubtree(ball);
	}
	ball = Ball::Create(glm::vec2(0.0,0.0));
	ball->SetScale(0.03);
	ball->SetAngle(0.0);
	ball->body->linearVelocity = glm::normalize(glm::vec2(0.0,-1.0))*BALL_VELOCITY;
	root->LinkChild(ball);
	SimplePhysics::RegisterSubtree(ball);
	balls_left--;
}

void UpdateStatusTexts(){
	balls_txt->SetText("Balls left: " + std::to_string(balls_left) + "/" + std::to_string(BALLS_MAX));
	bricks_txt->SetText("Bricks left: " + std::to_string(brick_count) + "/" + std::to_string(bricks_max));
}

void NewGame(){
	if(game_state == GAME_STATE_NOT_STARTED) return;
	balls_left = BALLS_MAX;
	CreateLevel();
	SpawnNewBall();
	UpdateStatusTexts();
	game_state = GAME_STATE_NOT_STARTED;
	overlay->SetActive(true);
	game_start_txt->SetActive(true);
	game_lost_txt->SetActive(false);
	game_won_txt->SetActive(false);
	game_restart_txt->SetActive(false);
}

void LooseGame(){
	game_state = GAME_STATE_LOST;
	overlay->SetActive(true);
	game_lost_txt->SetActive(true);
	game_restart_txt->SetActive(true);
}
void GameWon(){
	game_state = GAME_STATE_WON;
	overlay->SetActive(true);
	game_won_txt->SetActive(true);
	game_restart_txt->SetActive(true);
}

void BallLost(){
	if(game_state == GAME_STATE_WON) return;
	if(balls_left){
		// spawn a new ball
		SpawnNewBall();
		UpdateStatusTexts();
	}else{
		LooseGame();
	}
}

void StartGame(){
	game_start_txt->SetActive(false);
	overlay->SetActive(false);
	game_state = GAME_STATE_IN_PROGRESS;
}

int main(){
	srand(time(nullptr));

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	// This is the root object.
	root = Positionable::Create(glm::vec2(0.0,0.0));

	// Create background
	auto bg = Background::Create(0.1,8,12);
	root->LinkChild(bg);
	// Create board
	std::shared_ptr<Board> board = Board::Create();
	root->LinkChild(board);
	board->on_ball_lost.Subscribe( [](){BallLost();} );
	// Create overlay
	overlay = Overlay::Create();
	root->LinkChild(overlay);

	// Prepare the paddle
	auto paddle = Paddle::Create(glm::vec2(0.0,-SQRT3/2.0), PADDLE_SIZE);
	root->LinkChild(paddle);

	balls_txt = Text::Create("", glm::vec2(0,36), 36, glm::vec3(1.0,0.0,0.0),glm::vec2(-1.0,1.0));
	bricks_txt = Text::Create("", glm::vec2(0,36), 36, glm::vec3(1.0,0.4,0.0),glm::vec2(0.0,1.0));
	game_won_txt = Text::Create("You have won!!!", glm::vec2(0,36), 52,glm::vec3(0.0,0.9,0.0) ,glm::vec2(-0.35,-0.0));
	game_lost_txt = Text::Create("You have lost!", glm::vec2(0,36), 52,glm::vec3(0.9,0.0,0.0) ,glm::vec2(-0.33,-0.0));
	game_start_txt = Text::Create("Use arrow keys to start.", glm::vec2(0,36), 52,glm::vec3(0.9,0.7,0.0) ,glm::vec2(-0.55,-0.2));
	game_restart_txt = Text::Create("Press N to start a new game.", glm::vec2(0,36), 52,glm::vec3(0.9,0.7,0.0) ,glm::vec2(-0.6,-0.2));
	root->LinkChild(balls_txt);
	root->LinkChild(game_start_txt);
	root->LinkChild(game_lost_txt);
	root->LinkChild(game_won_txt);

	SimplePhysics::RegisterSubtree(root);

	// Reset balls count, re
	NewGame();

	double lasttime = glfwGetTime();
	// This is the main loop.
	do{
		double newtime = glfwGetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;
		if(game_state == GAME_STATE_IN_PROGRESS || game_state == GAME_STATE_WON){
			SimplePhysics::PerformIteration(time_delta);
			// Paddle movement
			float px = 0.0;
			if(Render::IsKeyPressed(GLFW_KEY_LEFT)) px += -1.0;
			if(Render::IsKeyPressed(GLFW_KEY_RIGHT)) px += 1.0;
			float newx = paddle->GetPosRelative().x + px * PADDLE_SPEED * time_delta;
			newx = glm::clamp(newx, -0.5f + PADDLE_SIZE, 0.5f - PADDLE_SIZE);
			paddle->SetPosRelative(glm::vec2(newx,-SQRT3/2.0));
		}
		if(game_state == GAME_STATE_NOT_STARTED){
			if(Render::IsKeyPressed(GLFW_KEY_RIGHT) || Render::IsKeyPressed(GLFW_KEY_LEFT) || Render::IsKeyPressed(GLFW_KEY_DOWN) || Render::IsKeyPressed(GLFW_KEY_UP))
				StartGame();
		}
		else if(game_state == GAME_STATE_WON || game_state == GAME_STATE_LOST || game_state == GAME_STATE_IN_PROGRESS){
			if(Render::IsKeyPressed(GLFW_KEY_N))
				NewGame();
		}

		// Draw a single frame.
		Render::Frame();


		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );
}
