#include "Node.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

glm::vec2 Rotate2dVector01(glm::vec2 v, float angle){
	glm::mat2 m(glm::cos(angle*2.0*M_PI), glm::sin(angle*-2.0*M_PI),
	            glm::sin(angle*2.0*M_PI), glm::cos(angle*2.0*M_PI));
	return m*v;
}

Node::Node(glm::vec3 relative_pos_){
	SetPosition(relative_pos_);
}

void Node::AddChild(std::shared_ptr<Node> ch){
	children.push_back(ch);
	ch->parent = shared_from_this();
}
void Node::SetParent(std::shared_ptr<Node> p){
	parent = p;
}
void Node::DetachFromParent(){
	std::shared_ptr<Node> pt = parent.lock();
	if(!pt) return;
	for(auto it = pt->children.begin(); it != pt->children.end(); it++){
		if((*it).get() == this){
			pt->children.erase(it);
			return;
		}
	}
	std::cerr << "Warning: child not detached from parent, because it is missing from parent children list" << std::endl;
}

bool Node::GetActive() const{
	return active;
}
void Node::SetActive(bool a){
	active = a;
}

void Node::SetPosition(glm::vec3 pos){
	position = pos;
	UpdateTransform();
}
void Node::SetScale(float s){
	scale = s;
	UpdateTransform();
}
void Node::SetRotation(glm::quat r){
	rotation = r;
	UpdateTransform();
}
glm::vec3 Node::GetPosition() const{
	return position;
}
float Node::GetScale() const{
	return scale;
}
glm::quat Node::GetRotation() const{
	return rotation;
}

glm::mat4 Node::GetTransform() const{
	return transform;
}
glm::mat4 Node::GetGlobalTransform() const{
	auto sp = parent.lock();
 	if(!sp) return GetTransform();
	return sp->GetGlobalTransform() * transform;
}

void Node::UpdateTransform(){
	glm::mat4 sc = glm::scale(glm::mat4(1.0),glm::vec3(scale));
	glm::mat4 tr = glm::translate(glm::mat4(1.0),position);
	glm::mat4 ro = glm::toMat4(rotation);
	transform = tr* ro * sc;
}

std::shared_ptr<Node> Node::Create(glm::vec3 pos){
	return std::make_shared<Node>(pos);
}


void NodeGroup::SetTop(std::shared_ptr<Node> t){
	top = t;
	Node::AddChild(top);
}

void NodeGroup::SetBottom(std::shared_ptr<Node> p){
	bottom = p;
}

void NodeGroup::AddChild(std::shared_ptr<Node> ch){
	if(bottom == nullptr) Node::AddChild(ch);
	else bottom->AddChild(ch);
}
