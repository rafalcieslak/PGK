#include "Render.hpp"
#include "Viewpoint.hpp"
#include "ObjParser.hpp"
#include "Text.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

bool mouse_left_down = false, mouse_right_down = false;
glm::vec2 drag_start_camera_pos;
glm::vec2 drag_start_light_pos;
float camera_pitch = 0.0, camera_yaw = 0.0;
float camera_start_pitch, camera_start_yaw;
float light_pitch = 0.0, light_yaw = 60.0;
float light_start_pitch, light_start_yaw;
float distance = 4.0 ,camera_distance;
std::shared_ptr<Viewpoint> camera;
float near, far;
glm::vec3 lightpos(100.0,100.0,100.0);
float zoom = 0;
bool auto_convert = false;

void update_camera_light_pos(){
	camera_yaw = std::max(-90.0f, std::min(90.0f, camera_yaw));
	light_yaw =  std::max(-90.0f, std::min(90.0f, light_yaw ));

	float scale = glm::cos(camera_yaw*0.0174532925);
	float xpos  = glm::sin(camera_pitch*0.0174532925);
	float zpos  = glm::cos(camera_pitch*0.0174532925);
	float ypos  = glm::sin(camera_yaw*0.0174532925);
	camera->SetPosition(camera_distance*glm::vec3(xpos*scale,ypos,scale*zpos));
	camera->LookAt(glm::vec3(0.0,0.0,0.0));

	near = camera_distance*0.1;
	far = distance*50;

	scale = glm::cos(light_yaw*0.0174532925);
	xpos  = glm::sin(light_pitch*0.0174532925);
	zpos  = glm::cos(light_pitch*0.0174532925);
	ypos  = glm::sin(light_yaw*0.0174532925);
	lightpos = camera_distance*glm::vec3(xpos*scale,ypos,scale*zpos);
}

void scroll_callback(double x){
	zoom -= x;
	camera_distance = glm::pow(1.1f,zoom)*distance;
	update_camera_light_pos();
}

void usage(){
	std::cout << "USAGE:   ./zadanie6 [-a] FILE.OBJ" << std::endl;
	std::cout << "   -a  - enables automatic conversion from unknown texture formats to bmp." << std::endl;
	std::cout << "         Requires the standard `convert` utility." << std::endl;
	exit(0);
}

int main(int argc, char** argv){
	if(argc < 2) usage();

	std::string objpath;
	if(std::string(argv[1]) == "-a"){
		if(argc < 3) usage();
		std::cout << "Auto conversion mode enabled." << std::endl;
		auto_convert = true;
		objpath = argv[2];
	}else{
		objpath = argv[1];
	}

	ObjParser p(objpath);
	if(!p.Good()){
		std::cout << "Problem opening file " << objpath << std::endl;
		return 0;
	}
	p.Parse();
	std::cout << "Parsed!" << std::endl;
	if(p.meshes.size() < 1){
		std::cout << "No meshes found." << std::endl;
		return 0;
	}

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	Render::scroll_callback = scroll_callback;

	bounds b;
	std::cout << "Mesh list: "<< std::endl;
	for(auto m : p.meshes){
		m->PrepareData();
		bounds d = m->GetBounds();
		b.Update(d.min); b.Update(d.max);
		std::cout << m->name << ": " << m->faces.size() << " triangles" <<  std::endl;
	}
	for(auto m : p.meshes){
		m->Translate( -b.Center() );
		m->PrepareBuffers();
	}
	camera_distance = 1.5* ( distance = b.Radius()*1.3f );
	std::partition(p.meshes.begin(), p.meshes.end(), [](const std::shared_ptr<Mesh> m){return m->material->alpha > 0.99;});

	camera = std::make_shared<Viewpoint>( glm::vec3(0.0, -10.0, 0.0) , glm::vec3(2.0,1.0,0.0));
	camera->SetAsActive();
	update_camera_light_pos();

	auto shaders_text = std::make_shared<Text>("1-4: select shader", glm::vec2(10,22),  16, glm::vec3(1.0,1.0,1.0));
	auto current_text = std::make_shared<Text>("Current shader: basic",  glm::vec2(10,42),  16, glm::vec3(1.0,1.0,1.0));
	auto movecam_text = std::make_shared<Text>("Mouse drag: rotate object/camera", glm::vec2(10,62),  16, glm::vec3(1.0,1.0,1.0));
	auto scroll_text  = std::make_shared<Text>("Mouse wheel: zoom in/out", glm::vec2(10,82),  16, glm::vec3(1.0,1.0,1.0));
	auto light_text   = std::make_shared<Text>("Right mouse drag: rotate light source", glm::vec2(10,102),  16, glm::vec3(1.0,1.0,1.0));
	auto     t_text   = std::make_shared<Text>("T: move light source to camera", glm::vec2(10,122),  16, glm::vec3(1.0,1.0,1.0));

	do{
		Render::Frame(p.meshes, lightpos, near, far, camera_distance);

		if(!mouse_left_down && Render::IsMouseDown(0)){
			// mouse button was just pressed
			drag_start_camera_pos = Render::GetMousePos();
			camera_start_pitch = camera_pitch;
			camera_start_yaw = camera_yaw;
			mouse_left_down = true;
		}else if(mouse_left_down && Render::IsMouseDown(0)){
			// mouse drag
			glm::vec2 drag = Render::GetMousePos();
			float dx = drag.x - drag_start_camera_pos.x;
			float dy = drag.y - drag_start_camera_pos.y;
			camera_pitch = camera_start_pitch - dx*200;
			camera_yaw   = camera_start_yaw   + dy*200;
			update_camera_light_pos();
		}else if(mouse_left_down && !Render::IsMouseDown(0)){
			mouse_left_down = false;
		}


		if(!mouse_right_down && Render::IsMouseDown(1)){
			// mouse button was just pressed
			drag_start_light_pos = Render::GetMousePos();
			light_start_pitch = light_pitch;
			light_start_yaw = light_yaw;
			mouse_right_down = true;
		}else if(mouse_right_down && Render::IsMouseDown(1)){
			// mouse drag
			glm::vec2 drag = Render::GetMousePos();
			float dx = drag.x - drag_start_light_pos.x;
			float dy = drag.y - drag_start_light_pos.y;
			if(glm::abs(dx) > glm::abs(dy)) dy = 0;
			else dx = 0;
			light_pitch = light_start_pitch - dx*200;
			light_yaw   = light_start_yaw   - dy*200;
			update_camera_light_pos();
		}else if(mouse_right_down && !Render::IsMouseDown(1)){
			mouse_right_down = false;
		}

		if(Render::IsKeyPressed(GLFW_KEY_T)){
			light_yaw = camera_yaw;
			light_pitch = camera_pitch;
			camera_distance = glm::pow(1.1f,zoom)*distance;
			update_camera_light_pos();
		}
		if(Render::IsKeyPressed(GLFW_KEY_1)) { Render::SetShaderMode(SHADER_MODE_BASIC); current_text->SetText("Current shader: basic"); }
		if(Render::IsKeyPressed(GLFW_KEY_2)) { Render::SetShaderMode(SHADER_MODE_TOON);  current_text->SetText("Current shader: toon "); }
		if(Render::IsKeyPressed(GLFW_KEY_3)) { Render::SetShaderMode(SHADER_MODE_TOON2); current_text->SetText("Current shader: toon2"); }
		if(Render::IsKeyPressed(GLFW_KEY_4)) { Render::SetShaderMode(SHADER_MODE_TOON3); current_text->SetText("Current shader: toon3"); }

	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
