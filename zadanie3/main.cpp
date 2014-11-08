#include <glm/glm.hpp>
#include "ModelBase.h"
#include "Board.h"
#include "Brick.h"
#include "Ball.h"
#include "Render.h"
#include "simplephysics.h"

int main(){
	// Prepare the renderer.
	int n = Render::Init();

	if(n) return n;
	glm::vec4 test;


	std::shared_ptr<Board> board = Board::Create();
	//std::shared_ptr<Brick> br = Brick::Create(glm::vec2(0.0,0.5));
	//std::shared_ptr<Brick> br2 = Brick::Create(glm::vec2(0.0,-1.2));
	std::shared_ptr<Ball> ba = Ball::Create(glm::vec2(0.485,-0.5));
	//br->SetScale(1.0);
	ba->SetScale(0.03);
	//br->SetAngle(-0.2);
	ba->SetAngle(0.0);
	ba->body->linearVelocity = glm::vec2(0.0,0.35);

	/*
	std::shared_ptr<Brick> br = Brick::Create(glm::vec2(0.0,0.5));
	std::shared_ptr<Ball> ba = Ball::Create(glm::vec2(-0.0,-0.0));
	ba->SetScale(0.03);
	*/

	double lasttime = glfwGetTime();
	// This is the main loop.
	do{
		double newtime = glfwGetTime();
		SimplePhysics::PerformIteration(newtime-lasttime);
		lasttime = newtime;

		// Draw a single frame.
		Render::Frame();

		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );
}
