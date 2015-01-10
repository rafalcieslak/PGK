#ifndef VIEWPOINT_HPP
#define VIEWPOINT_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <memory>

class Viewpoint : public std::enable_shared_from_this<Viewpoint>{
	glm::vec3 position;
	glm::vec3 direction;
	float fov = 1.396; // 80 degrees
	bool active = false;
public:
	Viewpoint(glm::vec3 position = glm::vec3(0.0), glm::vec3 direction = glm::vec3(0.0,0.1,0.0));
	float pitch = 0.0f, yaw = -0.0f;
	glm::vec3 GetDirection() const;
	void SetDirection(glm::vec3);
	void SetPosition(glm::vec3);
	glm::vec3 GetPosition() const;
	void SetAsActive();
	bool IsActive() const;
	void LookAt(glm::vec3);
	void LookAt(float x, float y, float z) {LookAt(glm::vec3(x,y,z));}
	void SetFOV(float f) {fov = f;}
	void SetFOVdg(float f) {fov = f*2.0*3.1415926/360.0;}
	float GetFOV() const {return fov;}
	float GetFOVdg() const {return fov*360.0/(2.0*3.1415926);}
	glm::mat4 GetTransform() const;
	void MoveForward(float t);
	void MoveBackward(float t);
	void StrafeLeft(float t);
	void StrafeRight(float t);
	void MovePitch(float delta);
	void MoveYaw(float delta);
	static std::shared_ptr<Viewpoint> active_viewpoint;
};

#endif //VIEWPOINT_HPP
