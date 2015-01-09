#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Node.hpp"

class Light : public Node{
public:
	Light(glm::vec3 pos, glm::vec3 c = glm::vec3(1.0,1.0,1.0));
	~Light();

	glm::vec3 color;
	float distance_influence = 0.2;
	float spatial_range = 10.0;
	glm::vec3 sda = glm::vec3(1.0,1.0,1.0);
	float multiplier = 1.0;
	float fixrange = 0.0;

	static std::vector<Light*> lights;
};

#endif // LIGHT_HPP
