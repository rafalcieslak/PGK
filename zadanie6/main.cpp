#include "Render.hpp"
#include "Viewpoint.hpp"
#include "ObjParser.hpp"
#include <iostream>
#include <cstdlib>


bool mouse_left_down = false, mouse_right_down = false;
glm::vec2 drag_start_camera_pos;
glm::vec2 drag_start_light_pos;
float camera_pitch = 0.0, camera_yaw = 0.0;
float camera_start_pitch, camera_start_yaw;
float light_pitch = 0.0, light_yaw = 0.0;
float light_start_pitch, light_start_yaw;
float distance = 4.0;
std::shared_ptr<Viewpoint> camera;
float near, far;
glm::vec3 lightpos(100.0,100.0,100.0);

void update_camera_light_pos(){
	camera_yaw = std::max(-90.0f, std::min(90.0f, camera_yaw));
	light_yaw =  std::max(-90.0f, std::min(90.0f, light_yaw ));

	float scale = glm::cos(camera_yaw*0.0174532925);
	float xpos  = glm::sin(camera_pitch*0.0174532925);
	float zpos  = glm::cos(camera_pitch*0.0174532925);
	float ypos  = glm::sin(camera_yaw*0.0174532925);
	camera->SetPosition(distance*glm::vec3(xpos*scale,ypos,scale*zpos));
	camera->LookAt(glm::vec3(0.0,0.0,0.0));

	scale = glm::cos(light_yaw*0.0174532925);
	xpos  = glm::sin(light_pitch*0.0174532925);
	zpos  = glm::cos(light_pitch*0.0174532925);
	ypos  = glm::sin(light_yaw*0.0174532925);
	lightpos = distance*glm::vec3(xpos*scale,ypos,scale*zpos);
}

void usage(){
	std::cout << "USAGE:   ./zadanie6 FILE.OBJ" << std::endl;
	exit(0);
}

int main(int argc, char** argv){
	if(argc < 2) usage();

	ObjParser p(argv[1]);
	p.Parse();
	std::cout << "Parsed!" << std::endl;

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	float max = 0.0;
	std::cout << "Mesh list: "<< std::endl;
	for(auto m : p.meshes){
		m->PrepareBuffers();
		float d = m->GetMaximumDistanceFromOriginSquared();
		if(d > max) max = d;
		std::cout << m->name << ": " << m->faces.size() << " triangles, size: " << d << std::endl;
	}
	distance = glm::pow(max,0.5f)*1.3;
	near = distance*0.01;
	far = distance*2;

	camera = std::make_shared<Viewpoint>( glm::vec3(0.0, -10.0, 0.0) , glm::vec3(2.0,1.0,0.0));
	camera->SetAsActive();
	update_camera_light_pos();

	do{
		Render::Frame(p.meshes, lightpos, near, far);

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
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
