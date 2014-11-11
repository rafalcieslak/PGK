#ifndef PADDLE_H
#define PADDLE_H

#include "../engine/Drawable.hpp"
#include "../engine/Body.hpp"


class Paddle : public PositionableGroup{
private:
	class PaddleDrawable : public Drawable{
	public:
		PaddleDrawable();
	};
	class BounceHighlight : public Drawable{
	public:
		BounceHighlight();
	};
	class PaddleBody : public StaticBody{
	public:
		PaddleBody();
		void init();
	};
	std::shared_ptr<PaddleDrawable> pd;
	std::shared_ptr<PaddleBody> pb;
	std::shared_ptr<BounceHighlight> bh;
	Paddle();
	void init(glm::vec2 pos, float size);
public:
	static std::shared_ptr<Paddle> Create(glm::vec2 pos = glm::vec2(0.0,0.0), float size = 0.15);
};
#endif //PADDLE_H
