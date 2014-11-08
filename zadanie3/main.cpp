#include <glm/glm.hpp>
#include "ModelBase.h"
#include "Board.h"
#include "Brick.h"
#include "Ball.h"
#include "Paddle.h"
#include "Render.h"
#include "simplephysics.h"

int main(){
	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	std::shared_ptr<Board> board = Board::Create();
	std::shared_ptr<Ball> ba = Ball::Create(glm::vec2(0.485,-0.5));
	ba->SetScale(0.03);
	ba->SetAngle(0.0);
	ba->body->linearVelocity = glm::normalize(glm::vec2(-0.45,0.65))*0.7f;

	auto paddle = Paddle::Create(glm::vec2(0.0,-SQRT3/2.0));
	const float paddle_speed = 0.4;

	auto temp_wall = Positionable::Create(glm::vec2(0.0,0.5));
	auto b1 = Brick::Create(glm::vec2(0.0,0.0),1);
	auto b2 = Brick::Create(glm::vec2(0.2,0.0),0);
	auto b3 = Brick::Create(glm::vec2(-0.2,0.0),0);
	temp_wall->LinkChild(b1);
	temp_wall->LinkChild(b2);
	temp_wall->LinkChild(b3);

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
		newx = glm::clamp(newx, -0.35f, 0.35f);
		paddle->SetPosRelative(glm::vec2(newx,-SQRT3/2.0));


		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );
}
