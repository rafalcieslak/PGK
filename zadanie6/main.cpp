#include "Render.hpp"
#include "Viewpoint.hpp"
#include <iostream>
#include <cstdlib>


bool mouse_down = false;
glm::vec2 drag_start_pos;
float pitch = 0.0, yaw = 0.0;
float start_pitch, start_yaw;
float distance = 4.0;
std::shared_ptr<Viewpoint> camera;

void update_camera_pos(){
	yaw = std::max(-90.0f, std::min(90.0f, yaw));

	float scale = glm::cos(yaw*0.0174532925);
	float xpos  = glm::sin(pitch*0.0174532925);
	float ypos  = glm::cos(pitch*0.0174532925);
	float zpos  = glm::sin(yaw*0.0174532925);

	camera->SetPosition(distance*glm::vec3(xpos*scale,ypos*scale,zpos));

	camera->LookAt(glm::vec3(0.0,0.0,0.0));
}

void usage(){

	exit(0);
}

int main(int argc, char** argv){
	if(argc < 3) usage();


	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto model = Model::Create(argv[1], argv[2]);
	if(!model) return -1;


	model->Prepare();

	camera = std::make_shared<Viewpoint>( glm::vec3(0.0, -10.0, 0.0) , glm::vec3(2.0,1.0,0.0));
	camera->SetAsActive();
	update_camera_pos();

	do{
		Render::Frame(model);

		if(!mouse_down && Render::IsMouseDown()){
			// mouse button was just pressed
			drag_start_pos = Render::GetMousePos();
			start_pitch = pitch;
			start_yaw = yaw;
			mouse_down = true;
		}else if(mouse_down && Render::IsMouseDown()){
			// mouse drag
			glm::vec2 drag = Render::GetMousePos();
			float dx = drag.x - drag_start_pos.x;
			float dy = drag.y - drag_start_pos.y;
			pitch = start_pitch + dx*200;
			yaw   = start_yaw   + dy*200;
			update_camera_pos();

		}else if(mouse_down && !Render::IsMouseDown()){

			mouse_down = false;
		}
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
