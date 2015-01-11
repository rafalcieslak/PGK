#include "Tile.hpp"
#include "Render.hpp"
#include "Text.hpp"
#include "Viewpoint.hpp"
#include <iostream>
#include <list>

std::vector<std::shared_ptr<Tile>> tiles;
std::shared_ptr<Viewpoint> ortho_camera, persp_camera;
std::shared_ptr<Text> fov_text;
short lod = 3;
bool tab_pressed = false, ctrl_pressed = false;
float light_intensity = 10.0;
float light_angle = 0.0;
bool auto_lod = false;
unsigned int last_lod_change_frames = 0;
#define MIN_FPS 25
#define MAX_FPS 90

enum {
	VIEWMODE_ORTHO,
	VIEWMODE_3D
} viewmode = VIEWMODE_ORTHO;

glm::vec4 FindCenter(){
	float xmax = -1000.0, xmin = 1000.0, ymax = -1000.0, ymin = 1000.0;
	for(auto t : tiles){
		xmin = std::min(xmin, (float)t->lon+0.5f);
		xmax = std::max(xmax, (float)t->lon+0.5f);
		ymin = std::min(ymin, (float)t->lat-0.5f);
		ymax = std::max(ymax, (float)t->lat-0.5f);
	}
	float xc = 0.5*(xmin+xmax);
	float yc = 0.5*(ymin+ymax);
	float xscale = cos(yc*0.0174532925);
	float range = std::max(xscale*(xmax-xmin),ymax-ymin);
	return glm::vec4(xscale*xc, yc, (range+1)/2.0, xscale);
}

void AddTileRange(int lat1, int lat2, int lon1, int lon2){
	for(int lat = lat1; lat < lat2; lat++)
		for(int lon = lon1; lon < lon2; lon++){
			auto t = Tile::Create(lat,lon);
			if(t) tiles.push_back(t);
		}
}
void ScrollCallback(double x){
	if(viewmode == VIEWMODE_3D){
		float fov = persp_camera->GetFOVdg();
		fov = glm::min(glm::max(fov - 5*x,50.0),150.0);
		persp_camera->SetFOVdg(fov);
		fov_text->SetText("Camera FOV: " + std::to_string(int(fov+0.5)));
	}else{
		float r = ortho_camera->ortho_range;
		r *= pow(0.7,x);
		ortho_camera->ortho_range = r;
	}
}

void AutoLODTooHigh(){
	if(last_lod_change_frames < 10) return;
	if(lod == 5) return;
	lod++;
	last_lod_change_frames = 0;
}
void AutoLODTooLow(){
	if(last_lod_change_frames < 10) return;
	if(lod == 0) return;
	lod--;
	last_lod_change_frames = 0;
}

inline bool intersect(float rAx1, float rAy1, float rAx2, float rAy2, float rBx1, float rBy1, float rBx2, float rBy2){
	return (rAx1 < rBx2 && rAx2 > rBx1 && rAy1 < rBy2 && rAy2 > rBy1);
}

int main(){
	std::cout << "Loading data..." << std::endl;
	AddTileRange(35,45,-10,0);

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;
	Tile::Init();

	auto lod_text = std::make_shared<Text>("1-6: set LOD, 0: auto", glm::vec2(10,34), 24, glm::vec3(1.0,1.0,1.0));
	auto tab_text = std::make_shared<Text>("TAB: Switch camera mode", glm::vec2(10,63), 24, glm::vec3(1.0,1.0,1.0));
	auto mov_text = std::make_shared<Text>("W/S/A/D: Move camera", glm::vec2(10,90), 24, glm::vec3(1.0,1.0,1.0));
	auto tri_text = std::make_shared<Text>("Triangles: ", glm::vec2(850,22), 16, glm::vec3(1.0,0.5,0.5));
	auto res_text = std::make_shared<Text>("Tile size: ", glm::vec2(850,42), 16, glm::vec3(1.0,0.5,0.5));
	auto rendertime_text = std::make_shared<Text>("Frame time: ", glm::vec2(850,62), 16, glm::vec3(1.0,0.5,0.5));
	auto fps_text = std::make_shared<Text>("FPS: ", glm::vec2(850,82), 16, glm::vec3(1.0,0.5,0.5));
	auto mouse_scroll_text = std::make_shared<Text>("Mouse wheel: Zoom in/out", glm::vec2(10,117), 24, glm::vec3(1.0,1.0,1.0));
	auto iop_text = std::make_shared<Text>("I/O/P: Set light contrast", glm::vec2(10,144), 24, glm::vec3(1.0,1.0,1.0));
	auto lig_text = std::make_shared<Text>("Light contrast: low", glm::vec2(700,22), 16, glm::vec3(0.5,0.5,1.0));
	auto lan_text = std::make_shared<Text>("Light angle: ", glm::vec2(700,42), 16, glm::vec3(0.5,0.5,1.0));
	     fov_text = std::make_shared<Text>("Camera FOV: 0", glm::vec2(700,62), 16, glm::vec3(0.5,0.5,1.0));

	glm::vec4 center = FindCenter();
	float xscale = center.w;
	ortho_camera = std::make_shared<Viewpoint>( glm::vec3(center.x, center.y, center.z) , glm::vec3(0.0,1.0,0.0));
	ortho_camera->ortho = true;
	ortho_camera->ortho_range = center.z;
	ortho_camera->yaw = -3.1415926f/2.0f;
	ortho_camera->SetAsActive();

	//persp_camera = std::make_shared<Viewpoint>( glm::vec3(center.x, center.y, center.z) , glm::vec3(0.0,1.0,0.0));
	persp_camera = std::make_shared<Viewpoint>( glm::vec3(1.0,1.0,1.0) , glm::vec3(0.0,1.0,0.0));
	persp_camera->pitch = 0.0;
	persp_camera->yaw = -3.1415926f/2.0f;

	std::cout << "Preparing VBOs..." << std::endl;
	for(auto tile : tiles) tile->Prepare();

	Render::scroll_callback = ScrollCallback;
	double lasttime = Render::GetTime();
	double sum10 = 0.0;
	std::list<double> times;
	do{
		// Measuring performance
		double newtime = Render::GetTime();
		double time_delta = newtime-lasttime;
		lasttime = newtime;
		times.push_front(time_delta);
		sum10 += time_delta;
		if(times.size() > 10){
			sum10 -= times.back();
			times.pop_back();
		}
		double avg10time = sum10/times.size();
		int avg10fps = 1.0/avg10time;
		if(auto_lod){
			if(avg10fps < MIN_FPS) AutoLODTooHigh();
			if(avg10fps > MAX_FPS) AutoLODTooLow();
			last_lod_change_frames++;
		}

		unsigned int triangles = 0;
		glm::vec2 campos(ortho_camera->GetPosition().x, ortho_camera->GetPosition().y);
		float camrange = ortho_camera->ortho_range;

		// Rendering the frame
		Render::FrameStart(light_intensity, light_angle*0.0174532925, xscale, viewmode==VIEWMODE_3D);
		for(auto tile : tiles){
			if(viewmode == VIEWMODE_ORTHO){
				if(!intersect(campos.x - camrange, campos.y - camrange, campos.x + camrange, campos.y + camrange,
							  tile->lon*xscale, tile->lat-1.0, (tile->lon+1.0)*xscale, tile->lat)) continue;
			}
			triangles += tile->Render(lod);
		}
		tri_text->SetText("Triangles: " + std::to_string(triangles));
		res_text->SetText("Tile resolution: " + std::to_string( Tile::GetTileResolution(lod)));
		rendertime_text->SetText("Frame time: " + std::to_string( avg10time ));
		fps_text->SetText("FPS: " + std::to_string( avg10fps ));
		Render::FrameEnd();

		// Processing user input
		if(viewmode == VIEWMODE_3D){
			time_delta *= 0.6;
			if(Render::IsKeyPressed(GLFW_KEY_W)) persp_camera->MoveForward(time_delta);
			if(Render::IsKeyPressed(GLFW_KEY_S)) persp_camera->MoveBackward(time_delta);
			if(Render::IsKeyPressed(GLFW_KEY_A)) persp_camera->StrafeLeft(time_delta);
			if(Render::IsKeyPressed(GLFW_KEY_D)) persp_camera->StrafeRight(time_delta);
			glm::vec2 mouse = Render::ProbeMouse();
			if(Render::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Render::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL)){
				light_angle -= mouse.x * 20;
				lan_text->SetText("Light angle: " + std::to_string((int(light_angle+0.5)+360*1000)%360));
			}else{
				persp_camera->MovePitch(mouse.x);
				persp_camera->MoveYaw(-mouse.y);
			}
			if(Render::IsKeyPressed(GLFW_KEY_TAB) && !tab_pressed){
				tab_pressed = true;
				// Switch to ortho camera
				viewmode = VIEWMODE_ORTHO;
				ortho_camera->SetAsActive();
				mouse_scroll_text->SetText("Mouse wheel: zoom in/out");
				fov_text->SetText("Camera FOV: 0");
			}else if(!Render::IsKeyPressed(GLFW_KEY_TAB)) tab_pressed = false;
		}else{
			float amount = time_delta * ortho_camera->ortho_range;
			if(Render::IsKeyPressed(GLFW_KEY_W)) ortho_camera->MoveNorth(amount);
			if(Render::IsKeyPressed(GLFW_KEY_S)) ortho_camera->MoveSouth(amount);
			if(Render::IsKeyPressed(GLFW_KEY_A)) ortho_camera->StrafeLeft(amount);
			if(Render::IsKeyPressed(GLFW_KEY_D)) ortho_camera->StrafeRight(amount);
			if(Render::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Render::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL)){
				glm::vec2 mouse = Render::ProbeMouse();
				light_angle -= mouse.x * 20;
				lan_text->SetText("Light angle: " + std::to_string((int(light_angle+0.5)+360*1000)%360));
			}
			if(Render::IsKeyPressed(GLFW_KEY_TAB) && !tab_pressed){
				tab_pressed = true;
				// Switch to persp camera
				viewmode = VIEWMODE_3D;
				persp_camera->SetAsActive();
				persp_camera->yaw = -3.1415926f/2.0f;
				mouse_scroll_text->SetText("Mouse wheel: Adjust FOV");
				fov_text->SetText("Camera FOV: " + std::to_string(int(persp_camera->GetFOVdg()+0.5)));
				Render::ProbeMouse(); // Resets mouse position to keep persp camera facing down
				persp_camera->yaw = -3.1415926f/2.0f;
			}else if(!Render::IsKeyPressed(GLFW_KEY_TAB)) tab_pressed = false;
		}
		if(Render::IsKeyPressed(GLFW_KEY_1)) {lod = 0; auto_lod = false;}
		if(Render::IsKeyPressed(GLFW_KEY_2)) {lod = 1; auto_lod = false;}
		if(Render::IsKeyPressed(GLFW_KEY_3)) {lod = 2; auto_lod = false;}
		if(Render::IsKeyPressed(GLFW_KEY_4)) {lod = 3; auto_lod = false;}
		if(Render::IsKeyPressed(GLFW_KEY_5)) {lod = 4; auto_lod = false;}
		if(Render::IsKeyPressed(GLFW_KEY_6)) {lod = 5; auto_lod = false;}
		if(Render::IsKeyPressed(GLFW_KEY_0)) auto_lod = true;

		if(Render::IsKeyPressed(GLFW_KEY_I)) { light_intensity = 4.0;   lig_text->SetText("Light contrast: high");}
		if(Render::IsKeyPressed(GLFW_KEY_O)) { light_intensity = 10.0;  lig_text->SetText("Light contrast: low");}
		if(Render::IsKeyPressed(GLFW_KEY_P)) { light_intensity = 100.0; lig_text->SetText("Light contrast: none");}


	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

	Render::CleanUp();
}
