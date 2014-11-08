#ifndef BOARD_H
#define BOARD_H

#include "Drawable.h"
#include "body.h"

class Board : public PositionableGroup{
private:
	class BoardDrawable : public Drawable{
	public:
		BoardDrawable();
	};
	class BoardBody : public DynamicBody{
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
