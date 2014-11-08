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

glm::vec2& Positionable::GetPosRelative(){
	return relative_pos;
}
glm::vec2& Positionable::GetScaleRelative(){
	return relative_scale;
}

void Positionable::SetPosRelative(glm::vec2 pos){
	relative_pos = pos;
}
void Positionable::SetScale(glm::vec2 scale){
	relative_scale = scale;
}

glm::vec2 Positionable::GetPos() const{
	return GetPosScale().xy();
}

glm::vec2 Positionable::GetScale() const{
	return GetPosScale().zw();
}

glm::vec4 Positionable::GetPosScale() const{
	if(parent == nullptr) return glm::vec4(relative_pos,relative_scale);
	glm::vec4 base = parent->GetPosScale();
	glm::vec2 mypos = base.xy() + (relative_pos * base.zw());
	glm::vec2 myscale = base.zw() * relative_scale;
	return glm::vec4(mypos, myscale);
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
