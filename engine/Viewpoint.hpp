#ifndef VIEWPOINT_HPP
#define VIEWPOINT_HPP

#include "Node.hpp"

class Viewpoint : public Node{

	glm::vec3 direction;
	float fov = 100.0;
	bool active = false;
public:
	Viewpoint(glm::vec3 position = glm::vec3(0.0), glm::vec3 direction = glm::vec3(0.0,0.1,0.0));
	glm::vec3 GetDirection() const;
	void SetDirection(glm::vec3);
	void SetAsActive();
	bool IsActive() const;
	void LookAt(glm::vec3);
	float GetFOV() const {return fov*2.0*3.14159265/360.0;}
	static std::shared_ptr<Viewpoint> active_viewpoint;
};

#endif //VIEWPOINT_HPP
