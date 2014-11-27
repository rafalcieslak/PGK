#include <glm/glm.hpp>
#include "../engine/Render.hpp"
#include "../engine/Viewpoint.hpp"
#include "Cube.hpp"
#include "../engine/Light.hpp"

int main(){
	srand(time(nullptr));

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto root = std::make_shared<Node>();

	auto cube = std::make_shared<Cube>(0.4);
	auto cube2 = std::make_shared<Cube>(0.2);
	cube2->SetPosition(glm::vec3(1.0,1.0,0.0));
	cube2->variant = 1;
	cube->AddChild(cube2);
	root->AddChild(cube);

	auto camera = std::make_shared<Viewpoint>(glm::vec3(0.0,0.8,2.0));
	camera->LookAt(glm::vec3(0.0,0.0,0.0));
	camera->SetAsActive();

	root->AddChild(camera);

	auto light = std::make_shared<Light>(glm::vec3(-1.5,0.9,1.5));
	root->AddChild(light);

	Render::SetRootNode(root);


	double lasttime = Render::GetTime();
	// This is the main loop.
	double p = 0.0;
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		p += 0.7 * time_delta;

		cube->SetRotation(glm::quat(glm::vec3(0.0,p,0.0)));

		Render::Frame();
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
