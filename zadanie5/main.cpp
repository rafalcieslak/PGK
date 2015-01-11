#include "Tile.hpp"
#include "Render.hpp"
#include "Text.hpp"
#include "Viewpoint.hpp"
#include <iostream>

std::vector<std::shared_ptr<Tile>> tiles;
short lod = 3;


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
	for(int lat = lat1; lat < lat2; lat++)
		for(int lon = lon1; lon < lon2; lon++){
			auto t = Tile::Create(lat,lon);
			if(t) tiles.push_back(t);
		}
}

int main(){
	std::cout << "Loading data..." << std::endl;
	AddTileRange(40,45,-5,0);

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;
	Tile::Init();

	auto lod_text = std::make_shared<Text>("1-6: set LOD", glm::vec2(10,34), 24, glm::vec3(1.0,1.0,1.0));
	auto tri_text = std::make_shared<Text>("Triangles: ", glm::vec2(850,22), 16, glm::vec3(1.0,0.5,0.5));
	auto res_text = std::make_shared<Text>("Tile size: ", glm::vec2(850,42), 16, glm::vec3(1.0,0.5,0.5));
	auto rendertime_text = std::make_shared<Text>("Frame time: ", glm::vec2(850,62), 16, glm::vec3(1.0,0.5,0.5));
	auto fps_text = std::make_shared<Text>("FPS: ", glm::vec2(850,82), 16, glm::vec3(1.0,0.5,0.5));

	glm::vec3 center = FindCenter();
	float xscale = cos(center.y*0.0174532925);
	auto v = std::make_shared<Viewpoint>( center , glm::vec3(0.0,1.0,0.0));
	v->yaw = -3.1415926f/2.0f;
	v->SetAsActive();

	std::cout << "Preparing VBOs..." << std::endl;
	for(auto tile : tiles) tile->Prepare();

	double lasttime = Render::GetTime();
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		Render::FrameStart();
		unsigned int triangles = 0;
		for(auto tile : tiles) triangles += tile->Render(lod, xscale);
		tri_text->SetText("Triangles: " + std::to_string(triangles));
		res_text->SetText("Tile resolution: " + std::to_string( Tile::GetTileResolution(lod)));
		rendertime_text->SetText("Frame time: " + std::to_string( time_delta ));
		fps_text->SetText("FPS: " + std::to_string( float(int((1.0/time_delta)*10))/10.0 ));
		Render::FrameEnd();

		time_delta *= 1.0;
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
