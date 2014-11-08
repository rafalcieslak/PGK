#ifndef PADDLE_H
#define PADDLE_H

#include "Drawable.h"
#include "body.h"


class Paddle : public PositionableGroup{
private:
	class PaddleDrawable : public Drawable{
	public:
		PaddleDrawable();
	};
	class PaddleBody : public StaticBody{
	public:
		PaddleBody();
		void init();
	};
	std::shared_ptr<PaddleDrawable> pd;
	std::shared_ptr<PaddleBody> pb;
	Paddle();
	void init(glm::vec2 pos);
public:
	static std::shared_ptr<Paddle> Create(glm::vec2 pos = glm::vec2(0.0,0.0));
};
#endif //PADDLE_H
