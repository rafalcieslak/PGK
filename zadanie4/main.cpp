#include <glm/glm.hpp>
#include "../engine/Render.hpp"
#include "Cube.hpp"

int main(){
	srand(time(nullptr));

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto cube = std::make_shared<Cube>(0.5);
	auto cube2 = std::make_shared<Cube>(0.42);
	cube2->SetPosition(glm::vec3(0.5,0.3,0.0));
	cube2->variant = 1;
	cube->variant = 0;
	cube->AddChild(cube2);
	cube2->SetActive(true);
	Render::SetRootNode(cube);
	double lasttime = Render::GetTime();
	// This is the main loop.
	double p = 0.0;
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		p += 1.5 * time_delta;

		cube->SetRotation(glm::quat(glm::vec3(0.0,p,0.0)));

		Render::Frame();
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
