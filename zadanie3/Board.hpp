#ifndef BOARD_H
#define BOARD_H

#include "engine/Drawable.hpp"
#include "engine/Body.hpp"

#define SQRT3 1.73205080757

class Board : public PositionableGroup{
private:
	class BoardDrawable : public Drawable{
	public:
		BoardDrawable();
	};
	class BoardBody : public StaticBody{
	public:
		BoardBody();
		void init();
	};
	class LooseArea : public StaticBody{
	public:
		LooseArea();
		void init();
	};
	std::shared_ptr<BoardDrawable> bd;
	std::shared_ptr<LooseArea> la;
	Board();
	void init();
public:
	std::shared_ptr<BoardBody> body;
	static std::shared_ptr<Board> Create();
	Signal<> on_ball_lost;
};

#endif //BOARD_H
