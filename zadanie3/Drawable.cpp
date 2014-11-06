#include "Drawable.h"
#include <algorithm>
#include <iostream>

std::vector<Drawable*> Drawable::drawables;
Drawable::Drawable(std::string id, glm::vec2 relative_pos):
		Positionable(relative_pos),  model_id(id){
	drawables.emplace_back(this);
}


Drawable::~Drawable(){
	auto p = std::find(drawables.begin(), drawables.end(), this);
	if (p != drawables.end()) // == vector.end() means the element was not found
	    drawables.erase(p);
}
