#include <glm/glm.hpp>
#include "../engine/Render.hpp"
#include "../engine/Viewpoint.hpp"
#include "Cube.hpp"
#include "Ball.hpp"
#include "Plane.hpp"
#include "Player.hpp"
#include "../engine/Light.hpp"
#include <iostream>

typedef enum viewModes{
	VIEW_MODE_THIRD_PERSON = 0,
	VIEW_MODE_FIRST_PERSON,
	VIEW_MODE_MAX
} viewModes;
viewModes view_mode = VIEW_MODE_THIRD_PERSON;

std::shared_ptr<Player> player;

void SwitchViewMode(){
	switch(view_mode){
		case VIEW_MODE_THIRD_PERSON:
			view_mode = VIEW_MODE_FIRST_PERSON;
			player->SwitchToFP();
			break;
		case VIEW_MODE_FIRST_PERSON:
			view_mode = VIEW_MODE_THIRD_PERSON;
			player->SwitchToTP();
			break;
		case VIEW_MODE_MAX:
			break;
	};
}

int main(){
	srand(time(nullptr));

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto root = std::make_shared<Node>();

	auto ball = std::make_shared<Ball>(0.9);
	auto cube2 = std::make_shared<Cube>(0.2);
	cube2->SetPosition(1.0,0.0,1.0);
	cube2->variant = 1;
	ball->AddChild(cube2);
	auto cube3 = std::make_shared<Cube>(0.3);
	cube3->SetScale(0.3,0.1,0.5);
	cube3->SetPosition(0.0,0.0,1.0);
	cube3->variant = 1;
	ball->AddChild(cube3);
	root->AddChild(ball);

	auto camera = std::make_shared<Viewpoint>(glm::vec3(0.0,2.0,0.8));
	camera->LookAt(0.0,0.0,0.0);
	camera->SetAsActive();

	root->AddChild(camera);

	auto light = std::make_shared<Light>(glm::vec3(-1.5,1.5,0.9));
	root->AddChild(light);

	player = Player::Create();
	player->SetPosition(-1.0,2.0,0.0);
	player->SwitchToTP();
	root->AddChild(player);

	auto fishtank = std::make_shared<Cube>(1.0);
	fishtank->SetScale(3.0,8.0,3.0);
	root->AddChild(fishtank);

	Render::SetRootNode(root);


	double lasttime = Render::GetTime();
	bool F_key_down = false;
	// This is the main loop.
	double p = 0.0;
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		p += 0.7 * time_delta;

		if(!Render::IsKeyPressed(GLFW_KEY_F)) F_key_down = false;
		else if(Render::IsKeyPressed(GLFW_KEY_F) && F_key_down == false){
			F_key_down = true;
			SwitchViewMode();
		}

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
