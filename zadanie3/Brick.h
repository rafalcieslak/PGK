#ifndef BRICK_H
#define BRICK_H

#include "Drawable.h"
#include "body.h"


class Brick : public PositionableGroup{
private:
	class BrickDrawable : public Drawable{
	public:
		BrickDrawable();
	};
	class BrickBody : public StaticBody{
	public:
		BrickBody();
		void init();
	};
	std::shared_ptr<BrickDrawable> bd;
	std::shared_ptr<BrickBody> bb;
	Brick();
	void init(glm::vec2 pos, unsigned int variant);
public:
	static std::shared_ptr<Brick> Create(glm::vec2 pos = glm::vec2(0.0,0.0), unsigned int variant = 0);
	Signal<> on_ball_collision;
	void Break();
};
#endif //BRICK_H
