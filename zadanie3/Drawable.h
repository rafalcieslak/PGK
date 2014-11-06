#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

class Drawable{
public:
	Drawable(std::string id, glm::vec2 position = glm::vec2(0.0,0.0), glm::vec2 s = glm::vec2(1.0,1.0));
	~Drawable();
	std::string model_id;
	glm::vec2 pos;
	glm::vec2 scale;
public:
	static std::vector<Drawable *> drawables;
};

#endif //DRAWABLE_H
