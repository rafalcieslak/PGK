#include "Drawable.hpp"
#include "Render.hpp"

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

void Drawable::StartAnimation(unsigned int mode, double len){
	anim_mode = mode;
	anim_length = len;
	anim_start = Render::GetTime();
}
