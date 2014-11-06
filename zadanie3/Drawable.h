#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Positionable.h"

class Drawable : public Positionable{
public:
	Drawable(std::string id, std::shared_ptr<Positionable> parent, glm::vec2 relative_pos);
	~Drawable();
	std::string model_id;
public:
	static std::vector<Drawable *> drawables;
};

#endif //DRAWABLE_H
