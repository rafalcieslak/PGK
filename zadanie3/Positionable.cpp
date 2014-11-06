#include "Positionable.h"

Positionable::Positionable(glm::vec2 relative_pos_) : relative_pos(relative_pos_){
}

void Positionable::LinkChild(std::shared_ptr<Positionable> ch){
	children.push_back(ch);
	ch->parent = shared_from_this();
}
void Positionable::SetParent(std::shared_ptr<Positionable> p){
	parent = p;
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

void PositionableGroup::SetTop(std::shared_ptr<Positionable> t){
	top = t;
	Positionable::LinkChild(top);
}

void PositionableGroup::SetBottom(std::shared_ptr<Positionable> p){
	bottom = p;
}

void PositionableGroup::LinkChild(std::shared_ptr<Positionable> ch){
	if(bottom == nullptr) Positionable::LinkChild(ch);
	else bottom->LinkChild(ch);
}
