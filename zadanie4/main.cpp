#include "../engine/Render.hpp"
#include "../engine/Viewpoint.hpp"
#include "Cube.hpp"
#include "Bubble.hpp"
#include "Player.hpp"
#include "../engine/Light.hpp"
#include "../engine/World.hpp"
#include "../engine/Text.hpp"
#include <iostream>
#include <set>
#include "main.hpp"

typedef enum viewModes{
	VIEW_MODE_THIRD_PERSON = 0,
	VIEW_MODE_FIRST_PERSON,
	VIEW_MODE_EXTERNAL,
	VIEW_MODE_MAX
} viewModes;
viewModes view_mode = VIEW_MODE_EXTERNAL;
bool pause = false;

std::shared_ptr<Player> player;
std::shared_ptr<ExternalCamera> external_cam;
std::shared_ptr<Node> bubble_node;

std::set<std::shared_ptr<Bubble>> bubbles;
std::set<std::shared_ptr<Bubble>> bubbles_to_pop;

std::shared_ptr<Text> text_fov;
std::shared_ptr<Text> text_level;
std::shared_ptr<Text> text_bubbles;

unsigned int bubble_limit = 30;
unsigned int current_level;

float random_float(float s){
	return s*((rand()%10000)/5000.0 - 1.0);
}

void UpdateFOVText(){
	float fov = Viewpoint::active_viewpoint->GetFOVdg();
	text_fov->SetText("Current FOV: " + std::to_string((int)(fov+0.5)));
}
void UpdateLevelText(){
	text_level->SetText("Level: " + std::to_string(current_level));
	text_bubbles->SetText("Bubbles: " + std::to_string(bubble_limit));
}
void SwitchViewMode(){
	switch(view_mode){
		case VIEW_MODE_EXTERNAL:
			view_mode = VIEW_MODE_FIRST_PERSON;
			player->SwitchToFP();
			break;
		case VIEW_MODE_FIRST_PERSON:
			view_mode = VIEW_MODE_THIRD_PERSON;
			player->SwitchToTP();
			break;
		case VIEW_MODE_THIRD_PERSON:
			view_mode = VIEW_MODE_EXTERNAL;
			external_cam->SetAsActive();
			break;
		case VIEW_MODE_MAX:
			break;
	};
	UpdateFOVText();
}

void spawn_new_bubble(){
	auto new_bubble = std::make_shared<Bubble>(BUBBLE_MAIN_SIZE);
	bubbles.emplace(new_bubble);
	float x = random_float(ROOM_SIZE_X/2.0);
	float y = random_float(ROOM_SIZE_Y/2.0);
	new_bubble->SetPosition(x,y, -ROOM_SIZE_Z/2.0);
	new_bubble->spatial = 3.1;
	new_bubble->variant = rand()%3;
	new_bubble->ApplyScale();
	bubble_node->AddChild(new_bubble);
}

void ScrollCallback(double x){
	float fov = Viewpoint::active_viewpoint->GetFOVdg();
	fov = glm::min(glm::max(fov - 5*x,50.0),150.0);
	Viewpoint::active_viewpoint->SetFOVdg(fov);
	UpdateFOVText();
}
bool VertexWon(glm::vec3 v){
	return v.y < -ROOM_SIZE_Y/2.0;
}
bool PlayerWon(){
	for(auto i : player->GetVerticesAbs()){
		if(VertexWon(i)) return true;
	}
	return false;
}

void PrepareLevel(int levelno){
	player->SetPosition(0.0,ROOM_SIZE_Y/2.0-0.3,0.0);
	player->ResetRotation();
	current_level = levelno;
	bubble_limit = 30 + levelno*5;
	UpdateLevelText();
}

bool IsVertexOutside(glm::vec3 p){
	return p.x < -ROOM_SIZE_X/2.0 ||
		   p.x >  ROOM_SIZE_X/2.0 ||
		   p.y < -ROOM_SIZE_Y/2.0 ||
		   p.y >  ROOM_SIZE_Y/2.0 ||
		   p.z < -ROOM_SIZE_Z/2.0 ||
		   p.z >  ROOM_SIZE_Z/2.0;
}
bool IsPlayerOutside(){
	for(auto i : player->GetVerticesAbs()){
		if(IsVertexOutside(i)) return true;
	}
	return false;
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

	external_cam = ExternalCamera::Create();
	external_cam->SetAsActive();

	root->AddChild(external_cam);

	auto main_light = std::make_shared<Light>(glm::vec3(ROOM_SIZE_X/2.0,ROOM_SIZE_Y/4.0,ROOM_SIZE_Z/2.0 + 1.0));
	root->AddChild(main_light);

	player = Player::Create();
	root->AddChild(player);

	bubble_node = std::make_shared<Node>();
	bubble_node->SetScale(1.0);
	root->AddChild(bubble_node);

	text_fov     = Text::Create("", glm::vec2(5,30),24,glm::vec3(1.0,0.0,0.0));
	text_level   = Text::Create("", glm::vec2(5,83),48,glm::vec3(0.0,1.0,0.0));
	text_bubbles = Text::Create("", glm::vec2(5,113),24,glm::vec3(1.0,1.0,0.0));
	UpdateFOVText();

	PrepareLevel(1);

	Render::SetRootNode(root);
	Render::scroll_callback = ScrollCallback;

	double lasttime = Render::GetTime();
	bool TAB_key_down = false;
	bool P_key_down = false;

	const double time_between_spawns = 10.0/bubble_limit;
	double time_to_next_spawn = time_between_spawns;

	// This is the main loop.
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		for(auto b : bubbles){
			if(!pause){
				b->ApplyMovement(time_delta);
				b->ApplyScale();
			}
			b->RotateTowards(Viewpoint::active_viewpoint->GetGlobalPos());
			if(b->ShouldPop()) bubbles_to_pop.insert(b);
		}
		for(auto b : bubbles_to_pop){
			bubbles.erase(b);
			bubble_node->RemoveChild(b);
			spawn_new_bubble();
		}
		bubbles_to_pop.clear();

		if(!pause){
			time_to_next_spawn -= time_delta;
			if(time_to_next_spawn <= 0.0){
				time_to_next_spawn = time_between_spawns;
				if(bubbles.size() < bubble_limit) spawn_new_bubble();
			}
		}

		// Bubble ordering for correct transparency.
		bubble_node->SortChildren([](std::shared_ptr<Node> n) -> float{
			auto b = std::dynamic_pointer_cast<Bubble>(n);
			return b->DistanceToCamera();
		});


		if(!Render::IsKeyPressed(GLFW_KEY_TAB)) TAB_key_down = false;
		else if(Render::IsKeyPressed(GLFW_KEY_TAB) && TAB_key_down == false){
			TAB_key_down = true;
			SwitchViewMode();
		}
		if(!Render::IsKeyPressed(GLFW_KEY_P)) P_key_down = false;
		else if(Render::IsKeyPressed(GLFW_KEY_P) && P_key_down == false){
			P_key_down = true;
			pause = !pause;
		}

		player->ClearMove();
		if(Render::IsKeyPressed(GLFW_KEY_W)) player->MoveForward();
		if(Render::IsKeyPressed(GLFW_KEY_S)) player->MoveBackward();
		if(Render::IsKeyPressed(GLFW_KEY_A)) player->StrafeLeft();
		if(Render::IsKeyPressed(GLFW_KEY_D)) player->StrafeRight();
		if(Render::IsKeyPressed(GLFW_KEY_LEFT)) player->MoveLeft();
		if(Render::IsKeyPressed(GLFW_KEY_RIGHT)) player->MoveRight();
		if(Render::IsKeyPressed(GLFW_KEY_UP)) player->MoveFront();
		if(Render::IsKeyPressed(GLFW_KEY_DOWN)) player->MoveBack();
		if(Render::IsKeyPressed(GLFW_KEY_PAGE_UP)) player->MoveUp();
		if(Render::IsKeyPressed(GLFW_KEY_PAGE_DOWN)) player->MoveDown();

		glm::vec2 mouse = Render::ProbeMouse();
		if(view_mode == VIEW_MODE_FIRST_PERSON || view_mode == VIEW_MODE_THIRD_PERSON){
			player->MovePitch(mouse.x);
			player->MoveYaw(mouse.y);
		}else if(view_mode == VIEW_MODE_EXTERNAL){
			external_cam->MovePitch(mouse.x);
			external_cam->MoveYaw(-mouse.y);
		}

		player->PerformMove(time_delta);
		if(!pause && PlayerWon()) PrepareLevel(current_level + 1);
		else if(!pause && IsPlayerOutside()) player->CancelMove();

		Render::Frame();
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
