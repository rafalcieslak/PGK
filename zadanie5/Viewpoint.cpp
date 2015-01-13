#include "Viewpoint.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

std::shared_ptr<Viewpoint> Viewpoint::active_viewpoint;

Viewpoint::Viewpoint(glm::vec3 _position, glm::vec3 direction){
	position = _position;
	SetDirection(direction);
}

void Viewpoint::SetDirection(glm::vec3 d){
	direction = glm::normalize(d);
}

glm::vec3 Viewpoint::GetDirection() const{
	return direction;
}

void Viewpoint::LookAt(glm::vec3 point){
	SetDirection(point - position);
}

void Viewpoint::SetAsActive(){
	if(active_viewpoint) active_viewpoint->active = false;
	active = true;
	active_viewpoint = std::dynamic_pointer_cast<Viewpoint>(shared_from_this());
}

void Viewpoint::SetPosition(glm::vec3 pos){
	position = pos;
}

glm::vec3 Viewpoint::GetPosition() const{
	return position;
}

glm::vec3 transformcoords(glm::vec3 pos){
    float q = cos(pos.y * 0.0174532925);
    return glm::vec3(sin(pos.x * 0.0174532925)*q,-cos(pos.x * 0.0174532925)*q,sin(pos.y * 0.0174532925))*pos.z;
}
glm::mat4 Viewpoint::GetTransform() const{
	glm::vec3 p = position;
	if(!ortho) p = transformcoords(position);
	glm::mat4 tr = glm::translate(glm::mat4(1.0),p);
	glm::quat pitch_quat = glm::angleAxis(pitch,glm::vec3(0.0f,0.0f,1.0f));
	glm::quat yaw_quat = glm::angleAxis(yaw,glm::rotate(pitch_quat,glm::vec3(1.0f,0.0f,0.0f)));
	glm::mat4 ro = glm::toMat4(rot * yaw_quat * pitch_quat);
	return tr * ro;
}
void Viewpoint::MoveNorth(float t){
	position += glm::vec3(0.0,1.0,0.0)*t;
}
void Viewpoint::MoveSouth(float t){
	position -= glm::vec3(0.0,1.0,0.0)*t;
}
void Viewpoint::MoveWest(float t){
	position += glm::vec3(1.0,0.0,0.0)*t;
}
void Viewpoint::MoveEast(float t){
	position -= glm::vec3(1.0,0.0,0.0)*t;
}
void Viewpoint::MoveIn(float t){
	position -= glm::vec3(0.0,0.0,1.0)*t;
	if(position.z < 1.001) position.z = 1.0004;
}
void Viewpoint::MoveOut(float t){
	position += glm::vec3(0.0,0.0,1.0)*t;
}
void Viewpoint::MovePitch(float delta){
	pitch -= delta;
}
void Viewpoint::MoveYaw(float delta){
	yaw = glm::clamp(yaw + delta,-3.1415926f/2.0f,3.1415926f/2.0f);
}
void Viewpoint::DownTo0(){
	rot = glm::rotation(glm::vec3(0.0,0.0,-1.0), glm::normalize(-transformcoords(position)));
}
