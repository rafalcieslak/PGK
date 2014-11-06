#ifndef BRICK_H
#define BRICK_H

#include "Drawable.h"
#include "body.h"

class Brick : public Drawable, public StaticBody{
public:
	Brick(std::shared_ptr<Positionable> parent = nullptr, glm::vec2 pos = glm::vec2(0.0,0.0));
};

#endif //BRICK_H
