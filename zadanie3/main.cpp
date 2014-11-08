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
	ba->body->linearVelocity = glm::vec2(0.15,0.65);

	auto paddle = Paddle::Create(glm::vec2(0.0,-SQRT3/2.0));
	const float paddle_speed = 0.4;

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
		newx = glm::clamp(newx, -0.38f, 0.38f);
		paddle->SetPosRelative(glm::vec2(newx,-SQRT3/2.0));


		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );
}
