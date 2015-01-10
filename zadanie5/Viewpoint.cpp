#include "Viewpoint.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

glm::mat4 Viewpoint::GetTransform() const{
	glm::mat4 tr = glm::translate(glm::mat4(1.0),position);
	glm::quat pitch_quat = glm::angleAxis(pitch,glm::vec3(0.0f,0.0f,1.0f));
	glm::quat yaw_quat = glm::angleAxis(yaw,glm::rotate(pitch_quat,glm::vec3(1.0f,0.0f,0.0f)));
	glm::mat4 ro = glm::toMat4(yaw_quat * pitch_quat);
	return tr * ro;
}
void Viewpoint::MoveForward(float t){
	glm::vec3 pitched_front = glm::rotate(glm::vec3(0.0,1.0,0.0),pitch,glm::vec3(0.0,0.0,1.0));
	glm::vec3 pitched_left = glm::rotate(glm::vec3(1.0,0.0,0.0),pitch,glm::vec3(0.0,0.0,1.0));
	glm::vec3 yawedpitched_front = glm::rotate(pitched_front,yaw,pitched_left);
	position += yawedpitched_front*t;
}
void Viewpoint::MoveBackward(float t){
	glm::vec3 pitched_front = glm::rotate(glm::vec3(0.0,1.0,0.0),pitch,glm::vec3(0.0,0.0,1.0));
	glm::vec3 pitched_left = glm::rotate(glm::vec3(1.0,0.0,0.0),pitch,glm::vec3(0.0,0.0,1.0));
	glm::vec3 yawedpitched_front = glm::rotate(pitched_front,yaw,pitched_left);
	position -= yawedpitched_front*t;
}
void Viewpoint::StrafeRight(float t){
	glm::vec3 pitched_left = glm::rotate(glm::vec3(1.0,0.0,0.0),pitch,glm::vec3(0.0,0.0,1.0));
	position += pitched_left*t;
}
void Viewpoint::StrafeLeft(float t){
	glm::vec3 pitched_left = glm::rotate(glm::vec3(1.0,0.0,0.0),pitch,glm::vec3(0.0,0.0,1.0));
	position -= pitched_left*t;
}
void Viewpoint::MovePitch(float delta){
	pitch -= delta;
}
void Viewpoint::MoveYaw(float delta){
	yaw = glm::clamp(yaw + delta,-3.1415926f/2.0f,3.1415926f/2.0f);
}
