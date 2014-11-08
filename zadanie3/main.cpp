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

	std::shared_ptr<Brick> b = Brick::Create(glm::vec2(0.0,0.5));
	std::shared_ptr<Ball> ba = Ball::Create(glm::vec2(-0.5,-0.5));
	b->SetScale(glm::vec2(1.0,1.0));
	ba->SetScale(glm::vec2(0.03,0.03));
	ba->body->linearVelocity = glm::vec2(0.25,0.25);

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
