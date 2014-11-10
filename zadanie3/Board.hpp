#ifndef BOARD_H
#define BOARD_H

#include "../engine/Drawable.hpp"
#include "../engine/Body.hpp"

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
	std::shared_ptr<BoardDrawable> bd;
	Board();
	void init();
public:
	std::shared_ptr<BoardBody> body;
	static std::shared_ptr<Board> Create();
};

#endif //BOARD_H
