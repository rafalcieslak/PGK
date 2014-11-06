#include "Drawable.h"
#include <algorithm>
#include <iostream>

std::vector<Drawable*> Drawable::drawables;

Drawable::Drawable(std::string id, glm::vec2 position, glm::vec2 s) : model_id(id), pos(position), scale(s){
	std::cerr << "Creating drawable" << std::endl;
	drawables.emplace_back(this);
}


Drawable::~Drawable(){
	auto p = std::find(drawables.begin(), drawables.end(), this);
	if (p != drawables.end()) // == vector.end() means the element was not found
	    drawables.erase(p);
}
