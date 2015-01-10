#include "Tile.hpp"
#include "Render.hpp"
#include "Text.hpp"
#include "Viewpoint.hpp"

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

int main(){
	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;
	Tile::Init();

	auto q = std::make_shared<Text>("AAAaaa", glm::vec2(50,50));
	tiles.push_back( Tile::CreateFromHGTFile(45,7) );
	tiles.push_back( Tile::CreateFromHGTFile(45,8) );

	auto v = std::make_shared<Viewpoint>( FindCenter() , glm::vec3(0.0,1.0,0.0));
	v->yaw = -3.1415926f/2.0f;
	v->SetAsActive();

	for(auto tile : tiles) tile->Prepare();

	double lasttime = Render::GetTime();
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		Render::FrameStart();
		for(auto tile : tiles) tile->Render();
		Render::FrameEnd();

		time_delta *= 0.3;
		if(Render::IsKeyPressed(GLFW_KEY_W)) v->MoveForward(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_S)) v->MoveBackward(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_A)) v->StrafeLeft(time_delta);
		if(Render::IsKeyPressed(GLFW_KEY_D)) v->StrafeRight(time_delta);

		glm::vec2 mouse = Render::ProbeMouse();
		v->MovePitch(mouse.x);
		v->MoveYaw(-mouse.y);

	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

	Render::CleanUp();
}
