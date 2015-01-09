#include "Tile.hpp"
#include "Render.hpp"
#include "Text.hpp"
#include "Viewpoint.hpp"

int main(){
	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto q = std::make_shared<Text>("AAAaaa", glm::vec2(50,50));
	Tile::Init();
	auto t = Tile::CreateFromHGTFile("/tmp/" + Tile::TileString(45,07) + ".hgt");
	t->Prepare();

	auto v = std::make_shared<Viewpoint>(glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,1.0,0.0));
	v->yaw = -3.1415926f/2.0f;
	v->SetAsActive();

	double lasttime = Render::GetTime();
	do{
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;

		Render::FrameStart();
		t->Render();
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
