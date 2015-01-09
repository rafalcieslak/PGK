#include "Viewpoint.hpp"

std::shared_ptr<Viewpoint> Viewpoint::active_viewpoint;

Viewpoint::Viewpoint(glm::vec3 position, glm::vec3 direction){
	SetPosition(position);
	SetDirection(direction);
}

void Viewpoint::SetDirection(glm::vec3 d){
	direction = glm::normalize(d);
}

glm::vec3 Viewpoint::GetDirection() const{
	return direction;
}

void Viewpoint::LookAt(glm::vec3 point){
	SetDirection(point - GetPosition());
}

void Viewpoint::SetAsActive(){
	if(active_viewpoint) active_viewpoint->active = false;
	active = true;
	active_viewpoint = std::dynamic_pointer_cast<Viewpoint>(shared_from_this());
}
