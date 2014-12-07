#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../engine/Node.hpp"
#include "../engine/Viewpoint.hpp"
#include "../engine/Light.hpp"
#include "Cube.hpp"
#include <list>

class Player : public NodeGroup{
	std::shared_ptr<Cube> player_cube;
	std::shared_ptr<Viewpoint> first_person_view;
	std::shared_ptr<Viewpoint> third_person_view;
	std::shared_ptr<Light> light;
	Player();
	void init();
	float speed = 1.2f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	glm::vec3 move = glm::vec3(0.0,0.0,0.0);
	glm::vec3 saved_pos;
	float saved_yaw, saved_pitch;
public:
	void SwitchToFP();
	void SwitchToTP();
	static std::shared_ptr<Player> Create();

	void ClearMove();
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void MoveFront();
	void MoveBack();
	void PerformMove(float delta);
	void CancelMove();

	void MovePitch(float delta);
	void MoveYaw(float delta);
	void ResetRotation();

	std::list<glm::vec3> GetVerticesAbs() const;
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
