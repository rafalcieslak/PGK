#include "Drawable.hpp"
#include <algorithm>
#include <iostream>

std::vector<Drawable*> Drawable::drawables;
Drawable::Drawable(std::string id, glm::vec2 relative_pos, unsigned int v):
		Positionable(relative_pos),  model_id(id), variant(v){
	drawables.emplace_back(this);
}


Drawable::~Drawable(){
	auto p = std::find(drawables.begin(), drawables.end(), this);
	if (p != drawables.end())
	    drawables.erase(p);
}
