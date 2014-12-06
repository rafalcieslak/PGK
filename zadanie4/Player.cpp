#include "Player.hpp"
#include "../engine/World.hpp"
#include <glm/gtx/rotate_vector.hpp>

Player::Player() {
	player_cube = std::make_shared<Cube>(0.2);
	player_cube->variant = 2;
	player_cube->culling = 1;
	first_person_view = std::make_shared<Viewpoint>(glm::vec3(0.0,0.0,0.0));
 	third_person_view = std::make_shared<Viewpoint>(glm::vec3(0.0,10.0,5.0));
	first_person_view->LookAt(glm::vec3(0.0,-1.0,0.0));
	third_person_view->LookAt(glm::vec3(0.0,0.0,0.0));
}

void Player::init(){
	AddChild(player_cube);
	player_cube->AddChild(first_person_view);
	player_cube->AddChild(third_person_view);
}

std::shared_ptr<Player> Player::Create(){
	auto p = std::shared_ptr<Player>(new Player());
	p->init();
	return p;
}
void Player::ClearMove(){
	move = glm::vec3(0.0,0.0,0.0);
	saved_pos = GetPosition();
	saved_pitch = pitch;
	saved_yaw = yaw;
}
void Player::MoveForward(){
	glm::vec3 pitched_front = glm::rotate(World::front,pitch,World::up);
	glm::vec3 pitched_left = glm::rotate(World::left,pitch,World::up);
	glm::vec3 yawedpitched_front = glm::rotate(pitched_front,yaw,pitched_left);
	move += yawedpitched_front;
}
void Player::MoveBackward(){
	glm::vec3 pitched_front = glm::rotate(World::front,pitch,World::up);
	glm::vec3 pitched_left = glm::rotate(World::left,pitch,World::up);
	glm::vec3 yawedpitched_front = glm::rotate(pitched_front,yaw,pitched_left);
	move -= yawedpitched_front;
}
void Player::StrafeRight(){
	glm::vec3 pitched_left = glm::rotate(World::left,pitch,World::up);
	move -= pitched_left;
}
void Player::StrafeLeft(){
	glm::vec3 pitched_left = glm::rotate(World::left,pitch,World::up);
	move += pitched_left;
}
void Player::MoveLeft(){ move += World::left; }
void Player::MoveRight(){ move += -World::left; }
void Player::MoveUp(){ move += World::up; }
void Player::MoveDown(){ move += -World::up; }
void Player::MoveFront(){ move += World::front; }
void Player::MoveBack(){ move += -World::front; }
void Player::PerformMove(float delta){
	if(glm::length(move) > 0.5)
		SetPosition(GetPosition() + glm::normalize(move)*delta*speed);
}
void Player::CancelMove(){
	SetPosition(saved_pos);
	pitch = saved_pitch;
	yaw = saved_yaw;
	player_cube->SetRotation(World::Rotation(pitch,yaw));
}

std::list<glm::vec3> Player::GetVerticesAbs() const{
	std::list<glm::vec3> res;
	glm::mat4 t = player_cube->GetGlobalTransform();
	res.push_back((t*glm::vec4( 1.0, 1.0, 1.0,1.0)).xyz());
	res.push_back((t*glm::vec4( 1.0, 1.0,-1.0,1.0)).xyz());
	res.push_back((t*glm::vec4( 1.0,-1.0, 1.0,1.0)).xyz());
	res.push_back((t*glm::vec4( 1.0,-1.0,-1.0,1.0)).xyz());
	res.push_back((t*glm::vec4(-1.0, 1.0, 1.0,1.0)).xyz());
	res.push_back((t*glm::vec4(-1.0, 1.0,-1.0,1.0)).xyz());
	res.push_back((t*glm::vec4(-1.0,-1.0, 1.0,1.0)).xyz());
	res.push_back((t*glm::vec4(-1.0,-1.0,-1.0,1.0)).xyz());
	return res;
}

void Player::MovePitch(float delta){
	pitch -= delta;
	player_cube->SetRotation(World::Rotation(pitch,yaw));
}
void Player::MoveYaw(float delta){
	yaw = glm::clamp(yaw + delta,-3.1415926f/2.0f,3.1415926f/2.0f);
	player_cube->SetRotation(World::Rotation(pitch,yaw));
}
void Player::ResetRotation(){
	pitch = yaw = 0;
	player_cube->SetRotation(World::Rotation(pitch,yaw));
}
void Player::SwitchToFP(){
	first_person_view->SetAsActive();
}
void Player::SwitchToTP(){
	third_person_view->SetAsActive();
}

//==================

ExternalCamera::ExternalCamera() {
 	the_cam = std::make_shared<Viewpoint>(glm::vec3(0.0,7.0,0.0));
	the_cam->LookAt(glm::vec3(0.0,0.0,0.0));
}

void ExternalCamera::init(){
	AddChild(the_cam);
}
std::shared_ptr<ExternalCamera> ExternalCamera::Create(){
	auto p = std::shared_ptr<ExternalCamera>(new ExternalCamera());
	p->init();
	return p;
}
void ExternalCamera::MovePitch(float delta){
	pitch -= delta;
	SetRotation(World::Rotation(pitch,yaw));
}
void ExternalCamera::MoveYaw(float delta){
	yaw = glm::clamp(yaw + delta,-3.1415926f/2.0f,3.1415926f/2.0f);
	SetRotation(World::Rotation(pitch,yaw));
}
