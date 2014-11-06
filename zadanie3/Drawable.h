#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>
#include <set>
#include <string>

class Drawable{
public:
	Drawable(std::string id, glm::vec2 position = glm::vec2(0.0,0.0));
	std::string model_id;
	glm::vec2 pos;
public:
	static std::set<std::shared_ptr<Drawable>> drawables;
};

#endif //DRAWABLE_H
