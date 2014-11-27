#include "Light.hpp"
std::vector<Light*> Light::lights;

Light::Light(glm::vec3 pos, glm::vec3 i){
	SetPosition(pos);
	intensity = i;
	lights.emplace_back(this);
}

Light::~Light(){
	auto p = std::find(lights.begin(), lights.end(), this);
	if (p != lights.end())
		lights.erase(p);
}
