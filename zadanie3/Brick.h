#ifndef BRICK_H
#define BRICK_H

#include "Drawable.h"
#include "body.h"

class BrickDrawable : public Drawable{
public:
	BrickDrawable(glm::vec2 pos = glm::vec2(0.0,0.0));
};

class BrickBody : public DynamicBody{
public:
	BrickBody( glm::vec2 relative_pos);
};

#endif //BRICK_H
