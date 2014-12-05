#ifndef BALL_HPP
#define BALL_HPP

#include "../engine/Drawable.hpp"

class Ball : public Drawable{
public:
	Ball(float scale = 1.0);
	void RotateTowards(glm::vec3);
};

#endif //BALL_HPP
