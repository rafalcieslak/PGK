#include "Tile.hpp"
#include "Render.hpp"
#include "Text.hpp"
#include "Viewpoint.hpp"
#include <iostream>

std::vector<std::shared_ptr<Tile>> tiles;

glm::vec3 FindCenter(){
	float xmax = -1000.0, xmin = 1000.0, ymax = -1000.0, ymin = 1000.0;
	for(auto t : tiles){
		xmin = std::min(xmin, (float)t->lon);
		xmax = std::max(xmax, (float)t->lon);
		ymin = std::min(ymin, (float)t->lat);
		ymax = std::max(ymax, (float)t->lat);
	}
	float xc = 0.5*(xmin+xmax);
	float yc = 0.5*(ymin+ymax);
	float range = std::max(xmax-xmin,ymax-ymin);
	return glm::vec3(xc, yc, range+1);
}

void AddTileRange(int lat1, int lat2, int lon1, int lon2){
	for(int lat = lat1; lat <= lat2; lat++)
		for(int lon = lon1; lon <= lon2; lon++){
			auto t = Tile::Create(lat,lon);
			if(t) tiles.push_back(t);
		}
}

int main(){

	std::cout << "Loading data..." << std::endl;
	AddTileRange(45,48,6,10);

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;
	Tile::Init();

	auto q = std::make_shared<Text>("1-6: set LOD", glm::vec2(10,34), 24, glm::vec3(1.0,1.0,1.0));


	auto v = std::make_shared<Viewpoint>( FindCenter() , glm::vec3(0.0,1.0,0.0));
	v->yaw = -3.1415926f/2.0f;
	v->SetAsActive();

	std::cout << "Preparing VBOs..." << std::endl;
	for(auto tile : tiles) tile->Prepare();

	short lod = 3;
	double lasttime = Render::GetTime();
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		Render::FrameStart();
		for(auto tile : tiles) tile->Render(lod);
		Render::FrameEnd();

		time_delta *= 0.3;
		if(Render::IsKeyPressed(GLFW_KEY_W)) v->MoveForward(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_S)) v->MoveBackward(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_A)) v->StrafeLeft(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_D)) v->StrafeRight(time_delta);

		if(Render::IsKeyPressed(GLFW_KEY_1)) lod = 0;
		if(Render::IsKeyPressed(GLFW_KEY_2)) lod = 1;
		if(Render::IsKeyPressed(GLFW_KEY_3)) lod = 2;
		if(Render::IsKeyPressed(GLFW_KEY_4)) lod = 3;
		if(Render::IsKeyPressed(GLFW_KEY_5)) lod = 4;
		if(Render::IsKeyPressed(GLFW_KEY_6)) lod = 5;

		glm::vec2 mouse = Render::ProbeMouse();
		v->MovePitch(mouse.x);
		v->MoveYaw(-mouse.y);

	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

	Render::CleanUp();
}
