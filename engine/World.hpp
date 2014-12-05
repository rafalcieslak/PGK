#ifndef WORLD_HPP
#define WORLD_HPP

#include <glm/glm.hpp>

// Several helper functions and objects that define how this particular world space should behave.
class World{
public:
	static glm::vec2 Rotate2dVector01(glm::vec2, float);
	static glm::quat Rotation(float pitch, float yaw);
	static const glm::vec3 up,front,left;
};
#endif //WORLD_HPP
