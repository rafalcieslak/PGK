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

#endif //PLAYER_HPP
