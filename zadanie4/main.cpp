#include "../engine/Render.hpp"
#include "../engine/Viewpoint.hpp"
#include "Cube.hpp"
#include "Bubble.hpp"
#include "Plane.hpp"
#include "Player.hpp"
#include "../engine/Light.hpp"
#include "../engine/World.hpp"
#include <iostream>
#include <set>
#include "main.hpp"

typedef enum viewModes{
	VIEW_MODE_THIRD_PERSON = 0,
	VIEW_MODE_FIRST_PERSON,
	VIEW_MODE_MAX
} viewModes;
viewModes view_mode = VIEW_MODE_THIRD_PERSON;

std::shared_ptr<Player> player;
std::shared_ptr<Node> bubble_node;

std::set<std::shared_ptr<Bubble>> bubbles;
std::set<std::shared_ptr<Bubble>> bubbles_to_pop;

float random_float(float s){
	return s*((rand()%10000)/5000.0 - 1.0);
}

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

void spawn_new_bubble(){
	auto new_bubble = std::make_shared<Bubble>(BUBBLE_MAIN_SIZE);
	bubbles.emplace(new_bubble);
	float x = random_float(ROOM_SIZE_X/2.0);
	float y = random_float(ROOM_SIZE_Y/2.0);
	new_bubble->SetPosition(x,y, -ROOM_SIZE_Z/2.0);
	bubble_node->AddChild(new_bubble);
}

int main(){
	srand(time(nullptr));

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto root = std::make_shared<Node>();

	auto fishtank = std::make_shared<Cube>(1.0);
	fishtank->SetScale(ROOM_SIZE_X/2.0,ROOM_SIZE_Y/2.0,ROOM_SIZE_Z/2.0);
	fishtank->culling = 2;
	root->AddChild(fishtank);


	auto cube2 = std::make_shared<Cube>(0.2);
	cube2->SetPosition(1.0,0.0,1.0);
	cube2->variant = 1;
	root->AddChild(cube2);
	auto cube3 = std::make_shared<Cube>(0.3);
	cube3->SetScale(0.3,0.1,0.5);
	cube3->SetPosition(0.0,0.0,1.0);
	cube3->variant = 1;
	root->AddChild(cube3);

	auto camera = std::make_shared<Viewpoint>(glm::vec3(0.0,2.0,0.8));
	camera->LookAt(0.0,0.0,0.0);
	camera->SetAsActive();

	root->AddChild(camera);

	auto light = std::make_shared<Light>(glm::vec3(0.0,0.0,3.0));
	root->AddChild(light);

	player = Player::Create();
	player->SetPosition(-1.0,2.0,0.0);
	player->SwitchToTP();
	root->AddChild(player);

	bubble_node = std::make_shared<Node>();
	bubble_node->SetScale(1.0);
	root->AddChild(bubble_node);

	Render::SetRootNode(root);

	double lasttime = Render::GetTime();
	bool F_key_down = false;
	// This is the main loop.
	double p = 0.0;
	spawn_new_bubble();
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		p += 0.7 * time_delta;
		cube2->SetRotation(World::Rotation(p,0));

		for(auto b : bubbles){
			b->ApplyMovement(time_delta);
			b->RotateTowards(Viewpoint::active_viewpoint->GetGlobalPos());
			if(b->ShouldPop()) bubbles_to_pop.insert(b);
		}
		for(auto b : bubbles_to_pop){
			bubbles.erase(b);
			bubble_node->RemoveChild(b);
			spawn_new_bubble();
		}
		bubbles_to_pop.clear();

		if(!Render::IsKeyPressed(GLFW_KEY_F)) F_key_down = false;
		else if(Render::IsKeyPressed(GLFW_KEY_F) && F_key_down == false){
			F_key_down = true;
			SwitchViewMode();
		}

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
