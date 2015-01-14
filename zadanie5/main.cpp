#include "Tile.hpp"
#include "Render.hpp"
#include "Text.hpp"
#include "Viewpoint.hpp"
#include <iostream>
#include <list>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

std::vector<std::shared_ptr<Tile>> tiles;
std::shared_ptr<Viewpoint> ortho_camera, persp_camera;
std::shared_ptr<Text> fov_text;
short lod = 3;
bool tab_pressed = false, ctrl_pressed = false, z_pressed = false, x_pressed = false;
float light_intensity = 10.0;
float light_angle = 0.0;
bool auto_lod = false;
float terrainscale = 1.0;
unsigned int last_lod_change_frames = 0;
#define MIN_FPS 25
#define MAX_FPS 55

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
	return glm::vec4(xc, yc, (range+1)/2.0, xscale);
}

void ScrollCallback(double x){
	if(viewmode == VIEWMODE_3D){
		persp_camera->MoveIn(x*(persp_camera->GetPosition().z-1.0f)*0.2);
	}else{
		float r = ortho_camera->ortho_range;
		r *= pow(0.7,x);
		ortho_camera->ortho_range = r;
	}
}

void AdjustFov(double x){
	float fov = persp_camera->GetFOVdg();
	fov = glm::min(glm::max(fov - 5*x,50.0),150.0);
	persp_camera->SetFOVdg(fov);
	fov_text->SetText("Camera FOV: " + std::to_string(int(fov+0.5)));
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

bool BlockToCoords(std::string str, int& /*out*/ lat, int& /*out*/ lon){
	if(str.length()!=7) return false;
	int n1 = std::stoi(str.substr(1,2));
	int n2 = std::stoi(str.substr(4,3));
	if(str[0] == 'S' || str[0] == 's') n1 *= -1;
	else if(str[0] == 'N' || str[0] == 'n') n1 *= 1;
	else return false;
	if(str[3] == 'W' || str[3] == 'w') n2 *= -1;
	else if(str[3] == 'E' || str[3] == 'e') n2 *= 1;
	else return false;
	lat = n1;
	lon = n2;
	return true;
}

void usage(){
	std::cout << "zadanie5 [-d USER_DIR [-n]] REGIONS | HGT_FILES" << std::endl;
	std::cout << "                                          " << std::endl;
	std::cout << "  HGT_FILES - list of .hgt files to be loaded.                         " << std::endl;
	std::cout << "  REGIONS   - list of map tiles to be loaded.                          " << std::endl;
	std::cout << "                 If the corresponding tile is available in USER_DIR,   " << std::endl;
	std::cout << "                 then that file will be used. Otherwise the application" << std::endl;
	std::cout << "                 will attempt to download the file from dds.cr.usgs.gov" << std::endl;
	std::cout << "                 Downloaded files are cached in /tmp.                  " << std::endl;
	std::cout << "                 A region can be a single tile, or a rectangular area. " << std::endl;
	std::cout << "                 For single tiles, the only accepted format is:        " << std::endl;
	std::cout << "                     [S|s|N|n]xx[W|w|E|e]yyy                           " << std::endl;
	std::cout << "                 where xx represents the latitude, and yyy longitude.  " << std::endl;
	std::cout << "                 For example, N37W010 is a correct tile.               " << std::endl;
	std::cout << "                 For rectangular areas, the only accepted format is:   " << std::endl;
	std::cout << "                     [S|s|N|n]xx[W|w|E|e]yyy-[S|s|N|n]zz[W|w|E|e]www   " << std::endl;
	std::cout << "                 where xx, yyy, zz and www represent the area edges.   " << std::endl;
	std::cout << "                 For example, N35W010-N45E000 is a correct area.       " << std::endl;
	std::cout << "-d USER_DIR - patch to a custom directory containing .hgt files.       " << std::endl;
	std::cout << "                 Useful if you have already downloaded the data.       " << std::endl;
	std::cout << "                 This directory will be used only to look for REGIONS  " << std::endl;
	std::cout << "                 files, it has no effect on HGT_FILES.                 " << std::endl;
	std::cout << "                 Nothing is ever written into this directory.          " << std::endl;
	std::cout << "         -n - Never perform any downloads, relay ONLY on USER_DIR.     " << std::endl;
	exit(0);
}

int main(int argc, char** argv){
	std::string userdir = "";
	bool neverdownload = false;

	// Parse arguments
	if(argc == 1) usage();
	int lat, lon;
	for(int i = 1; i < argc; i++){
		std::string arg(argv[i]);
		int k = arg.length();
		if(arg == "-d"){
			i++;
			if(i == argc){
				std::cout << "Parameter for -d is missing." << std::endl;
				exit(0);
			}
			userdir = argv[i];
		}else if(arg == "-n"){
			if(userdir == ""){
				std::cout << "Downloads disabled, but no user dir was speficied. This makes no sense!" << std::endl;
				exit(0);
			}
			neverdownload = true;
		}else if(arg.length() >= 4 && arg.substr(k-4) == ".hgt"){
			// This is a .hgt file
			if(arg.length() < 11 || !BlockToCoords(arg.substr(k-11,7),lat,lon)){
				std::cout << "Argument '" << arg << "' does not seem valid." << std::endl;
				exit(0);
			}
			std::cout << "Opening file " << arg << "..." << std::endl;
			std::shared_ptr<Tile> t = Tile::Create(arg,lat,lon);
			if(!t){
				std::cout << "Problem with accessing file " << arg << "." << std::endl;
				exit(0);
			}
			tiles.push_back(t);
		}else if(k == 7){
			// single tile
			if(!BlockToCoords(arg,lat,lon)){
				std::cout << "Argument '" << arg << "' does not seem valid." << std::endl;
				exit(0);
			}
			std::shared_ptr<Tile> t = Tile::Create(lat,lon,userdir,neverdownload);
			if(t) tiles.push_back(t);
		}else if(k == 15){
			// single tile
			int lat1, lon1;
			if(!BlockToCoords(arg.substr(0,7),lat1,lon1)){
				std::cout << "Argument '" << arg << "' does not seem valid." << std::endl;
				exit(0);
			}
			if(!BlockToCoords(arg.substr(8,7),lat,lon)){
				std::cout << "Argument '" << arg << "' does not seem valid." << std::endl;
				exit(0);
			}
			for(int x = lat1; x < lat; x++){
				for(int y = lon1; y < lon; y++){
					std::shared_ptr<Tile> t = Tile::Create(x,y,userdir,neverdownload);
					if(t) tiles.push_back(t);
				}
			}
		}else{
			std::cout << "Argument '" << arg << "' does not seem valid." << std::endl;
			exit(0);
		}
	}
	std::cout << "Loading data..." << std::endl;

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;
	Tile::Init();

	// Preapare UT text labels
	auto lod_text = std::make_shared<Text>("1-6: set LOD, 0: auto",              glm::vec2(10,22),  16, glm::vec3(1.0,1.0,1.0));
	auto tab_text = std::make_shared<Text>("TAB: Switch camera mode",            glm::vec2(10,42),  16, glm::vec3(1.0,1.0,1.0));
	auto mov_text = std::make_shared<Text>("W/S/A/D or mouse drag: Move camera", glm::vec2(10,62),  16, glm::vec3(1.0,1.0,1.0));
	auto shi_text = std::make_shared<Text>("Hold SHIFT for faster movements",    glm::vec2(10,82),  16, glm::vec3(1.0,1.0,1.0));
	auto mouse_scroll_text = std::make_shared<Text>("Mouse wheel: Zoom in/out",  glm::vec2(10,102), 16, glm::vec3(1.0,1.0,1.0));
	auto rca_text = std::make_shared<Text>("R: Reset camera",                    glm::vec2(10,122), 16, glm::vec3(1.0,1.0,1.0));
	auto rot_text = std::make_shared<Text>("E+mouse: Rotate lightning",          glm::vec2(10,142), 16, glm::vec3(1.0,1.0,1.0));
	auto iop_text = std::make_shared<Text>("I/O/P: Set light contrast",          glm::vec2(10,162), 16, glm::vec3(1.0,1.0,1.0));
	auto jkl_text = std::make_shared<Text>("J/K/L: Switch terrain scale",        glm::vec2(10,182), 16, glm::vec3(1.0,1.0,1.0));
	auto adjfov_text = std::make_shared<Text>("",                                glm::vec2(10,202), 16, glm::vec3(1.0,1.0,1.0));

	auto tri_text = std::make_shared<Text>("Triangles: ",         glm::vec2(830,22), 16, glm::vec3(1.0,0.5,0.5));
	auto res_text = std::make_shared<Text>("Tile size: ",         glm::vec2(830,42), 16, glm::vec3(1.0,0.5,0.5));
	auto rendertime_text = std::make_shared<Text>("Frame time: ", glm::vec2(830,62), 16, glm::vec3(1.0,0.5,0.5));
	auto fps_text = std::make_shared<Text>("FPS: ",               glm::vec2(830,82), 16, glm::vec3(1.0,0.5,0.5));

	auto lig_text = std::make_shared<Text>("Light contrast: low",      glm::vec2(670,22), 16, glm::vec3(0.5,0.5,1.0));
	auto lan_text = std::make_shared<Text>("Light angle: ",            glm::vec2(670,42), 16, glm::vec3(0.5,0.5,1.0));
	     fov_text = std::make_shared<Text>("Camera FOV: 0",            glm::vec2(670,62), 16, glm::vec3(0.5,0.5,1.0));
	auto tsc_text = std::make_shared<Text>("Terrain scale: realistic", glm::vec2(670,82), 16, glm::vec3(0.5,0.5,1.0));

	// Setup cameras
	glm::vec4 center = FindCenter();
	float xscale = center.w;
	ortho_camera = std::make_shared<Viewpoint>( glm::vec3(center.x*xscale, center.y, 1.5) , glm::vec3(0.0,1.0,0.0));
	ortho_camera->ortho = true;
	ortho_camera->ortho_range = center.z;
	ortho_camera->yaw = -3.1415926f/2.0f;
	ortho_camera->SetAsActive();

	persp_camera = std::make_shared<Viewpoint>( glm::vec3(center.x, center.y, 1.15) , glm::vec3(0.0,1.0,0.0));
	persp_camera->pitch = 0.0;
	persp_camera->yaw = -3.1415926f/2.0f;

	// Prepare tile VBO data
	std::cout << "Preparing VBOs..." << std::endl;
	for(auto tile : tiles) tile->Prepare();

	Render::scroll_callback = ScrollCallback;
	double lasttime = Render::GetTime();
	double sum10 = 0.0;
	std::list<double> times;
	// The main loop
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
		Render::FrameStart(light_intensity, light_angle*0.0174532925, xscale, viewmode==VIEWMODE_3D, terrainscale);
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
			float amount = time_delta*22.0*(persp_camera->GetPosition().z-1.0f);
			if(Render::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) amount *= 3;
			if(Render::IsKeyPressed(GLFW_KEY_W)) persp_camera->MoveForward(amount);
			if(Render::IsKeyPressed(GLFW_KEY_S)) persp_camera->MoveBackward(amount);
			if(Render::IsKeyPressed(GLFW_KEY_A)) persp_camera->MoveRight(amount);
			if(Render::IsKeyPressed(GLFW_KEY_D)) persp_camera->MoveLeft(amount);
			if(Render::IsKeyPressed(GLFW_KEY_Z) && !z_pressed) {z_pressed = true; AdjustFov(1);}
			else if(!Render::IsKeyPressed(GLFW_KEY_Z)) z_pressed = false;
			if(Render::IsKeyPressed(GLFW_KEY_X) && !x_pressed) {x_pressed = true; AdjustFov(-1);}
			else if(!Render::IsKeyPressed(GLFW_KEY_X)) x_pressed = false;
			glm::vec2 mouse = Render::ProbeMousePos();
			if(Render::IsKeyPressed(GLFW_KEY_E)){
				light_angle -= mouse.x * 20;
				lan_text->SetText("Light angle: " + std::to_string((int(light_angle+0.5)+360*1000)%360));
			}else if(Render::IsMouseDown()){
				// Dragging the map in 3d mode.
				glm::vec3 v = glm::rotate(glm::vec3(mouse.x * 30.0*(persp_camera->GetPosition().z-1.0f), mouse.y * 30.0*(persp_camera->GetPosition().z-1.0f), 0.0)
											* (Render::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)?3.0f:1.0f),
										  -persp_camera->pitch,
										  glm::vec3(0.0,0.0,1.0));
				persp_camera->MoveEast (v.x);
				persp_camera->MoveNorth(v.y);
			}else {
				persp_camera->MovePitch(mouse.x);
				persp_camera->MoveYaw(-mouse.y);
			}
			if(Render::IsKeyPressed(GLFW_KEY_TAB) && !tab_pressed){
				tab_pressed = true;
				// Switch to ortho camera
				viewmode = VIEWMODE_ORTHO;
				ortho_camera->SetAsActive();
				fov_text->SetText("Camera FOV: 0");
				adjfov_text->SetText("");
				jkl_text->SetText("");
			}else if(!Render::IsKeyPressed(GLFW_KEY_TAB)) tab_pressed = false;
		}else{
			float amount = time_delta * ortho_camera->ortho_range * 0.66;
			if(Render::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) amount *= 3;
			if(Render::IsKeyPressed(GLFW_KEY_W)) ortho_camera->MoveNorth(amount);
			if(Render::IsKeyPressed(GLFW_KEY_S)) ortho_camera->MoveSouth(amount);
			if(Render::IsKeyPressed(GLFW_KEY_A)) ortho_camera->MoveEast(amount);
			if(Render::IsKeyPressed(GLFW_KEY_D)) ortho_camera->MoveWest(amount);
			glm::vec2 mouse = Render::ProbeMousePos();
			if(Render::IsKeyPressed(GLFW_KEY_E)){
				light_angle -= mouse.x * 20;
				lan_text->SetText("Light angle: " + std::to_string((int(light_angle+0.5)+360*1000)%360));
			}else if(Render::IsMouseDown()){
				float q = Render::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)?3.0:1.0;
				ortho_camera->MoveEast (mouse.x * camrange * q);
				ortho_camera->MoveNorth(mouse.y * camrange * q);
			}
			if(Render::IsKeyPressed(GLFW_KEY_TAB) && !tab_pressed){
				tab_pressed = true;
				// Switch to persp camera
				viewmode = VIEWMODE_3D;
				persp_camera->SetAsActive();
				fov_text->SetText("Camera FOV: " + std::to_string(int(persp_camera->GetFOVdg()+0.5)));
				adjfov_text->SetText("Z/X: Adjust FOV");
				jkl_text->SetText("J/K/L: Switch terrain scale");
				Render::ProbeMousePos(); // Resets mouse position to keep persp camera facing down
				persp_camera->yaw = -3.141592653/2.0;
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
		if(Render::IsKeyPressed(GLFW_KEY_J)) { terrainscale = 1.0;   tsc_text->SetText("Terrain scale: realistic");}
		if(Render::IsKeyPressed(GLFW_KEY_K)) { terrainscale = 3.0;  tsc_text->SetText("Terrain scale: x3");}
		if(Render::IsKeyPressed(GLFW_KEY_L)) { terrainscale = 8.0;  tsc_text->SetText("Terrain scale: x8");}
		if(Render::IsKeyPressed(GLFW_KEY_R)) {
			ortho_camera->SetPosition(glm::vec3(center.x*xscale, center.y, 1.5));
			ortho_camera->ortho_range = center.z;
			Render::ProbeMousePos();
			persp_camera->SetPosition(glm::vec3(center.x, center.y, 1.15));
			persp_camera->pitch = 0.0;
			persp_camera->yaw = -3.141592653/2.0;
		}
		persp_camera->DownTo0();

	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

	Render::CleanUp();
}
