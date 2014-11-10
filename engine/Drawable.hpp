#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Positionable.hpp"

class Drawable : public Positionable{
public:
	Drawable(std::string id, glm::vec2 relative_pos = glm::vec2(0.0,0.0), unsigned int variant = 0);
	~Drawable();
	std::string model_id;
	unsigned int variant;
	static std::vector<Drawable *> drawables;
};

#endif //DRAWABLE_H
