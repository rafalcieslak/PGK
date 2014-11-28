#include <glm/glm.hpp>
#include "../engine/Render.hpp"
#include "../engine/Viewpoint.hpp"
#include "Cube.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "../engine/Light.hpp"
#include <iostream>

int main(){
	srand(time(nullptr));

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto root = std::make_shared<Node>();

	auto ball = std::make_shared<Ball>(0.9);
	auto cube2 = std::make_shared<Cube>(0.2);
	cube2->SetPosition(glm::vec3(1.0,0.0,1.0));
	cube2->variant = 1;
	ball->AddChild(cube2);
	auto cube3 = std::make_shared<Cube>(0.3);
	cube3->SetPosition(glm::vec3(0.0,0.0,1.0));
	cube3->variant = 1;
	ball->AddChild(cube3);
	root->AddChild(ball);

	auto camera = std::make_shared<Viewpoint>(glm::vec3(0.0,2.0,0.8));
	camera->LookAt(glm::vec3(0.0,0.0,0.0));
	camera->SetAsActive();

	root->AddChild(camera);

	auto light = std::make_shared<Light>(glm::vec3(-1.5,1.5,0.9));
	root->AddChild(light);

	auto player = Player::Create();
	player->SetPosition(glm::vec3(-1.0,2.0,0.0));
	player->SwitchToTP();
	root->AddChild(player);

	Render::SetRootNode(root);


	double lasttime = Render::GetTime();
	// This is the main loop.
	double p = 0.0;
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		p += 0.7 * time_delta;

		cube2->SetRotation(glm::quat(glm::vec3(p,0.0,0.0)));
		if(Render::IsKeyPressed(GLFW_KEY_W)) player->MoveForward(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_S)) player->MoveBackward(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_A)) player->StrafeLeft(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_D)) player->StrafeRight(time_delta);

		glm::vec2 mouse = Render::ProbeMouse();
		player->MovePitch(mouse.x);
		player->MoveYaw(mouse.y);

		Render::Frame();
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
