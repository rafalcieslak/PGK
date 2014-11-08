#include <glm/glm.hpp>
#include "ModelBase.h"
#include "Brick.h"
#include "Ball.h"
#include "Render.h"
#include "simplephysics.h"

int main(){
	// Prepare the renderer.
	int n = Render::Init();

	if(n) return n;
	glm::vec4 test;
	/*
	std::shared_ptr<BrickBody> bb = std::make_shared<BrickBody>(glm::vec2(-0.2,-0.2));
	std::shared_ptr<BrickDrawable> bd = std::make_shared<BrickDrawable>(glm::vec2(0.4,0.4));
	bb->LinkChild(bd);
	bb->linearVelocity = glm::vec2(-0.1,0.1);
	*/

	std::shared_ptr<Brick> b = Brick::Create(glm::vec2(0.1,0.3));
	std::shared_ptr<Ball> ba = Ball::Create(glm::vec2(-0.1,-0.3));


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
