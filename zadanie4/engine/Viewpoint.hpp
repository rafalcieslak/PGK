#ifndef VIEWPOINT_HPP
#define VIEWPOINT_HPP

#include "Node.hpp"

class Viewpoint : public Node{

	glm::vec3 direction;
	float fov = 1.745; // 100 degrees
	bool active = false;
public:
	Viewpoint(glm::vec3 position = glm::vec3(0.0), glm::vec3 direction = glm::vec3(0.0,0.1,0.0));
	glm::vec3 GetDirection() const;
	void SetDirection(glm::vec3);
	void SetAsActive();
	bool IsActive() const;
	void LookAt(glm::vec3);
	void LookAt(float x, float y, float z) {LookAt(glm::vec3(x,y,z));}
	void SetFOV(float f) {fov = f;}
	void SetFOVdg(float f) {fov = f*2.0*3.1415926/360.0;}
	float GetFOV() const {return fov;}
	float GetFOVdg() const {return fov*360.0/(2.0*3.1415926);}
	static std::shared_ptr<Viewpoint> active_viewpoint;
};

#endif //VIEWPOINT_HPP
