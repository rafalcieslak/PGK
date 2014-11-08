#ifndef BALL_H
#define BALL_H

#include "Drawable.h"
#include "body.h"
#include <array>

#define BALL_PTS 30

class Ball : public PositionableGroup{
private:
	class BallDrawable : public Drawable{
	public:
		static std::array<float,BALL_PTS*6> ball_vertices;
		static std::array<float,BALL_PTS*9> ball_colors;
		BallDrawable();
	};
	class BallBody : public DynamicBody{
	public:
		BallBody();
	};
	std::shared_ptr<BallDrawable> bd;
	Ball();
	void init(glm::vec2 pos);
public:
	std::shared_ptr<BallBody> body;
	static std::shared_ptr<Ball> Create(glm::vec2 pos = glm::vec2(0.0,0.0));
};

#endif //BALL_H
