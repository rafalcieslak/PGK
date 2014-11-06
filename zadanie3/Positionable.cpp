#include "Positionable.h"

Positionable::Positionable(std::shared_ptr<Positionable> parent_, glm::vec2 relative_pos_) : parent(parent_), relative_pos(relative_pos_){
	if(parent != nullptr){
		parent->AddChild(shared_from_this());
	}
}

void Positionable::AddChild(std::shared_ptr<Positionable> ch){
	children.push_back(ch);
}

glm::vec2 Positionable::GetPosRelative(){
	return relative_pos;
}
glm::vec2& Positionable::PosRelative(){
	return relative_pos;
}
void Positionable::SetPosRelative(glm::vec2 pos){
	relative_pos = pos;
}

glm::vec2 Positionable::GetPos(){
	if(parent == nullptr) return relative_pos;
	return parent->GetPos() + relative_pos;
}
