#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../engine/Node.hpp"
#include "../engine/Viewpoint.hpp"
#include "Cube.hpp"

class Player : public NodeGroup{
	std::shared_ptr<Cube> player_cube;
	std::shared_ptr<Viewpoint> first_person_view;
	std::shared_ptr<Viewpoint> third_person_view;
	Player();
	void init();
	float speed = 1.2f;
	float pitch = 0.0f;
	float yaw = 0.0f;
public:
	void SwitchToFP();
	void SwitchToTP();
	static std::shared_ptr<Player> Create();
	void MoveForward(float delta);
	void MoveBackward(float delta);
	void StrafeLeft(float delta);
	void StrafeRight(float delta);
	void MovePitch(float delta);
	void MoveYaw(float delta);
};

class ExternalCamera : public NodeGroup{
private:
	ExternalCamera();
	void init();
	std::shared_ptr<Viewpoint> the_cam;
	float pitch = 0.0f;
	float yaw = 0.0f;
public:
	static std::shared_ptr<ExternalCamera> Create();
	void MovePitch(float delta);
	void MoveYaw(float delta);
	void SetAsActive() {the_cam->SetAsActive();}
};
#endif //PLAYER_HPP
