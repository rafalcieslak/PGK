#include <glm/glm.hpp>
#include "Board.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Background.hpp"
#include "Paddle.hpp"
#include "../engine/Render.hpp"
#include "../engine/SimplePhysics.hpp"

#define PADDLE_SIZE 0.12f

int main(){
	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto root = Positionable::Create(glm::vec2(0.0,0.0));

	auto bg = Background::Create(0.1,8,12);

	std::shared_ptr<Board> board = Board::Create();
	std::shared_ptr<Ball> ball = Ball::Create(glm::vec2(0.0,0.0));
	ball->SetScale(0.03);
	ball->SetAngle(0.0);
	ball->body->linearVelocity = glm::normalize(glm::vec2(0.0,-1.0))*0.7f;

	auto paddle = Paddle::Create(glm::vec2(0.0,-SQRT3/2.0), PADDLE_SIZE);
	const float paddle_speed = 0.4;

	auto temp_wall = Positionable::Create(glm::vec2(0.0,0.5));
	auto b1 = Brick::Create(glm::vec2(-0.4,0.0),0); temp_wall->LinkChild(b1);
	auto b2 = Brick::Create(glm::vec2(-0.2,0.0),2); temp_wall->LinkChild(b2);
	auto b3 = Brick::Create(glm::vec2( 0.0,0.0),1); temp_wall->LinkChild(b3);
	auto b4 = Brick::Create(glm::vec2( 0.2,0.0),0); temp_wall->LinkChild(b4);
	auto b5 = Brick::Create(glm::vec2( 0.4,0.0),1); temp_wall->LinkChild(b5);

	root->LinkChild(board);
	root->LinkChild(paddle);
	root->LinkChild(ball);
	root->LinkChild(temp_wall);

	SimplePhysics::RegisterSubtree(root);

	double lasttime = glfwGetTime();
	// This is the main loop.
	do{
		double newtime = glfwGetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;
		SimplePhysics::PerformIteration(time_delta);

		// Draw a single frame.
		Render::Frame();

		// Paddle movement
		float px = 0.0;
		if(Render::IsKeyPressed(GLFW_KEY_LEFT)) px += -1.0;
		if(Render::IsKeyPressed(GLFW_KEY_RIGHT)) px += 1.0;
		float newx = paddle->GetPosRelative().x + px * paddle_speed * time_delta;
		newx = glm::clamp(newx, -0.5f + PADDLE_SIZE, 0.5f - PADDLE_SIZE);
		paddle->SetPosRelative(glm::vec2(newx,-SQRT3/2.0));


		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );
}
