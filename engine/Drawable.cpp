#include "Drawable.hpp"
#include "Render.hpp"

Drawable::Drawable(std::string id, glm::vec3 relative_pos, unsigned int v):
		Node(relative_pos),  model_id(id), variant(v){

}


Drawable::~Drawable(){
}

void Drawable::StartAnimation(unsigned int mode, double len){
	anim_mode = mode;
	anim_length = len;
	anim_start = Render::GetTime();
}
