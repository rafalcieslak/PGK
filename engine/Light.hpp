#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Node.hpp"

class Light : public Node{
	glm::vec3 intensity;
public:
	Light(glm::vec3 pos, glm::vec3 intensity = glm::vec3(1.0,1.0,1.0));
	~Light();

	static std::vector<Light*> lights;
};

#endif // LIGHT_HPP
