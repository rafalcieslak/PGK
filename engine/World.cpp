#include "World.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

const glm::vec3 World::front(0.0,-1.0,0.0);
const glm::vec3 World::left(1.0, 0.0,0.0);
const glm::vec3 World::up(0.0, 0.0,1.0);

glm::vec2 World::Rotate2dVector01(glm::vec2 v, float angle){
	glm::mat2 m(glm::cos(angle*2.0*M_PI), glm::sin(angle*-2.0*M_PI),
	            glm::sin(angle*2.0*M_PI), glm::cos(angle*2.0*M_PI));
	return m*v;
}

glm::quat World::Rotation(float pitch, float yaw){
	glm::quat pitch_quat = glm::angleAxis(pitch,glm::vec3(0.0f,0.0f,1.0f));
	glm::quat yaw_quat = glm::angleAxis(yaw,glm::rotate(pitch_quat,glm::vec3(1.0f,0.0f,0.0f)));
	return yaw_quat * pitch_quat;
}
