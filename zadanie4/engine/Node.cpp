#include "Node.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Node::Node(glm::vec3 relative_pos_){
	SetPosition(relative_pos_);
}

void Node::AddChild(std::shared_ptr<Node> ch){
	children.push_back(ch);
	ch->parent = shared_from_this();
}
void Node::RemoveChild(std::shared_ptr<Node> ch){
	ch->parent.reset();
	auto it = std::find(children.begin(), children.end(), ch);
	if(it == children.end()){
		std::cerr << "Error: Trying to remove a non-existent child!" << std::endl;
	}else{
		children.erase(it);
	}
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
void Node::SetScale(glm::vec3 s){
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
glm::vec3 Node::GetScale() const{
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
	glm::mat4 sc = glm::scale(glm::mat4(1.0),scale);
	glm::mat4 tr = glm::translate(glm::mat4(1.0),position);
	glm::mat4 ro = glm::toMat4(rotation);
	transform = tr* ro * sc;
}

void Node::SortChildren(std::function<float(std::shared_ptr<Node>)> value_func){
	std::sort(children.begin(),children.end(), [value_func](std::shared_ptr<Node> a, std::shared_ptr<Node> b) -> bool {
		return value_func(a) > value_func(b);
	});
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
void NodeGroup::RemoveChild(std::shared_ptr<Node> ch){
	if(bottom == nullptr) Node::RemoveChild(ch);
	else bottom->RemoveChild(ch);
}
