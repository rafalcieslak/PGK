#include "Player.hpp"
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

glm::vec3 Player::front(0.0,-1.0,0.0);
glm::vec3 Player::left(1.0, 0.0,0.0);
glm::vec3 Player::up(0.0, 0.0,1.0);

Player::Player() {
	player_cube = std::make_shared<Cube>(0.2);
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

void Player::MoveForward(float delta){
	glm::vec3 pitched_front = glm::rotate(front,pitch,up);
	glm::vec3 pitched_left = glm::rotate(left,pitch,up);
	glm::vec3 yawedpitched_front = glm::rotate(pitched_front,yaw,pitched_left);
	SetPosition(GetPosition() + speed*delta*yawedpitched_front);
}
void Player::MoveBackward(float delta){
	glm::vec3 pitched_front = glm::rotate(front,pitch,up);
	glm::vec3 pitched_left = glm::rotate(left,pitch,up);
	glm::vec3 yawedpitched_front = glm::rotate(pitched_front,yaw,pitched_left);
	SetPosition(GetPosition() - speed*delta*yawedpitched_front);
}
void Player::StrafeRight(float delta){
	glm::vec3 pitched_left = glm::rotate(left,pitch,up);
	SetPosition(GetPosition() - speed*delta*pitched_left);
}
void Player::StrafeLeft(float delta){
	glm::vec3 pitched_left = glm::rotate(left,pitch,up);
	SetPosition(GetPosition() + speed*delta*pitched_left);
}

void Player::MovePitch(float delta){
	pitch -= delta;
	UpdateRotation();
}
void Player::MoveYaw(float delta){
	yaw = glm::clamp(yaw + delta,-3.1415926f/2.0f,3.1415926f/2.0f);
	UpdateRotation();
}
void Player::UpdateRotation(){
	glm::quat pitch_quat = glm::angleAxis(pitch,glm::vec3(0.0f,0.0f,1.0f));
	glm::quat yaw_quat = glm::angleAxis(yaw,glm::rotate(pitch_quat,glm::vec3(1.0f,0.0f,0.0f)));
	player_cube->SetRotation( yaw_quat * pitch_quat);
}
