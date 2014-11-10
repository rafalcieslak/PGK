#include <glm/glm.hpp>
#include "Board.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Background.hpp"
#include "Paddle.hpp"
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

typedef enum GameState{
	GAME_STATE_NOT_STARTED,
	GAME_STATE_IN_PROGRESS,
	GAME_STATE_WON,
	GAME_STATE_LOST
} GameState;

std::shared_ptr<Ball> ball;
std::shared_ptr<Positionable> root, level;
std::shared_ptr<Text> balls_txt;
unsigned int balls_left;
GameState game_state;

void AddBrick(int x, int y, int variant, std::shared_ptr<Positionable> parent){
	float xoffset = 0.0;
	if((y>0?y:-y)%2) xoffset = BRICK_WIDTH/2.0;
	auto brick = Brick::Create(glm::vec2(x*BRICK_WIDTH + xoffset, y*BRICK_HEIGHT),variant,BRICK_HEIGHT);
	parent->LinkChild(brick);
}


void CreateLevel(){
	level = Positionable::Create();
	for(int y = 0; y < 5; y++){
		for(int x = -4+y/2; x <= 4-y/2; x++){
			if(y%2!=1 || x<4-y/2)
				AddBrick(x,y,rand()%6,level);
		}
	}
	level->SetPosRelative(glm::vec2(0.0,0.3));
	root->LinkChild(level);
	SimplePhysics::RegisterSubtree(level);
}


void SpawnNewBall(){
	if(ball) ball->DetachFromParent();
	ball = Ball::Create(glm::vec2(0.0,0.0));
	ball->SetScale(0.03);
	ball->SetAngle(0.0);
	ball->body->linearVelocity = glm::normalize(glm::vec2(0.0,-1.0))*BALL_VELOCITY;
	root->LinkChild(ball);
	SimplePhysics::RegisterSubtree(ball);
	balls_left--;
}


void UpdateBallsText(){
	balls_txt->SetText("Balls left " + std::to_string(balls_left) + "/" + std::to_string(BALLS_MAX));
}

void NewGame(){
	balls_left = BALLS_MAX;
	SpawnNewBall();
	UpdateBallsText();
	game_state = GAME_STATE_NOT_STARTED;
}

void LooseGame(){
	std::cout << "GAME LOST" << std::endl;
	NewGame();
}

void BallLost(){
	std::cout << "BALL LOST" << std::endl;
	if(balls_left){
		// spawn a new ball
		SpawnNewBall();
		UpdateBallsText();
	}else{
		LooseGame();
	}
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

	// Prepare the paddle
	auto paddle = Paddle::Create(glm::vec2(0.0,-SQRT3/2.0), PADDLE_SIZE);
	root->LinkChild(paddle);

	balls_txt = Text::Create("", glm::vec2(0,36), 36, glm::vec3(1.0,0.0,0.0),glm::vec2(-1.0,1.0));
	root->LinkChild(balls_txt);

	SimplePhysics::RegisterSubtree(root);


	// Create the level
	CreateLevel();

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

		// Draw a single frame.
		Render::Frame();


		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );
}
