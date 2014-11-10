#include "Positionable.h"

glm::vec2 Rotate2dVector01(glm::vec2 v, float angle){
	glm::mat2 m(glm::cos(angle*2.0*M_PI), glm::sin(angle*-2.0*M_PI),
	            glm::sin(angle*2.0*M_PI), glm::cos(angle*2.0*M_PI));
	return m*v;
}

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
float& Positionable::GetScaleRelative(){
	return relative_scale;
}

bool& Positionable::GetActiveRelative(){
	return active;
}
bool Positionable::GetActiveAbsolute(){
	if(parent.expired()) return active;
	if(!active) return false;
	auto p2 = parent.lock();
	if(!p2->GetActiveAbsolute()) return false;
	return active;
}

void Positionable::SetActive(bool a){
	active = a;
}

void Positionable::SetPosRelative(glm::vec2 pos){
	relative_pos = pos;
}
void Positionable::SetPosAbsolute(glm::vec2 pos){
	glm::vec4 current_posscale_angle = GetPosScaleAngle();
	glm::vec2 diff = pos - current_posscale_angle.xy();
	diff = Rotate2dVector01(diff,-1.0*current_posscale_angle.w);
	diff.x /= current_posscale_angle.z;
	diff.y /= current_posscale_angle.z;
	relative_pos += diff;
}

void Positionable::SetScale(float scale){
	relative_scale = scale;
}
void Positionable::SetAngle(float angle){
	relative_angle = angle;
}


glm::vec2 Positionable::GetPos() const{
	return GetPosScaleAngle().xy();
}

float Positionable::GetScale() const{
	return GetPosScaleAngle().z;
}
float Positionable::GetAngle() const{
	return GetPosScaleAngle().w;
}

glm::vec4 Positionable::GetPosScaleAngle() const{
	if(parent.expired()) return glm::vec4(relative_pos,relative_scale,relative_angle);
	auto p2 = parent.lock();
	glm::vec4 base = p2->GetPosScaleAngle();
	glm::vec2 mypos = base.xy() + base.z * Rotate2dVector01(relative_pos, base.w);
	float myscale = base.z * relative_scale;
	float myangle = base.w + relative_angle;
	return glm::vec4(mypos, myscale, myangle);
}

std::shared_ptr<Positionable> Positionable::Create(glm::vec2 pos){
	return std::make_shared<Positionable>(pos);
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
