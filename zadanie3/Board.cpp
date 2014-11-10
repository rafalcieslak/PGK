#include "Board.hpp"
#include "../engine/ModelBase.hpp"
#include <iostream>

#define BOARD_COLOR 0.0, 0.0, 0.7, 1.0

Board::BoardDrawable::BoardDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("Board")){
		ModelBase::GetInstance().AddModelTriangles("Board", 6,
		               { 1.0, 1.0, 1.0, 0.0, 0.0, SQRT3,
		                -1.0, 1.0,-1.0, 0.0,-0.0, SQRT3,
		       	  	     0.0, SQRT3, 0.5, SQRT3/2.0, -0.5, SQRT3/2.0,
						 1.0, 0.0, 1.0,-1.0, 0.0, -SQRT3,
						-1.0, 0.0,-1.0,-1.0,-0.0, -SQRT3,
					/*0.0,-SQRT3, 0.5,-SQRT3/2.0, -0.5,-SQRT3/2.0,*/},
		               { BOARD_COLOR, BOARD_COLOR, BOARD_COLOR,
		                 BOARD_COLOR, BOARD_COLOR, BOARD_COLOR,
						 BOARD_COLOR, BOARD_COLOR, BOARD_COLOR,
						 BOARD_COLOR, BOARD_COLOR, BOARD_COLOR,
						 BOARD_COLOR, BOARD_COLOR, BOARD_COLOR,
					/*0.0,0.05,0.2, 0.0,0.05,0.2, 0.0,0.05,0.2,*/});
	}
	model_id = "Board";
}

Board::BoardBody::BoardBody(){

}

void Board::BoardBody::init(){
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(0.0, 1.0), glm::vec2(0.5,1.0-SQRT3/2.0));
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(1.5, SQRT3/2.0), glm::vec2(0.5,SQRT3/2.0), 1.0/6.0);
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(-1.5, SQRT3/2.0), glm::vec2(0.5,SQRT3/2.0), -1.0/6.0);
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(1.5, -SQRT3/2.0), glm::vec2(0.8,SQRT3/2.0), -1.0/6.0);
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(-1.5, -SQRT3/2.0), glm::vec2(0.8,SQRT3/2.0), 1.0/6.0);
}

Board::LooseArea::LooseArea(){

}

void Board::LooseArea::init(){
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(0.0,-2.0), glm::vec2(1.0,0.85));
}

Board::Board(){
	bd = std::make_shared<BoardDrawable>();
	body = std::make_shared<BoardBody>();
	la = std::make_shared<LooseArea>();
	body->init();
	la->init();
}

void Board::init(){
	body->LinkChild(bd);
	body->LinkChild(la);
	SetTop(body);
	SetBottom(bd);
	la->on_collision.Subscribe( [this](std::shared_ptr<Body> body){
		if(body->id == "ball"){
			this->on_ball_lost.Happen();
		}
	});
}

std::shared_ptr<Board> Board::Create(){
	auto b = std::shared_ptr<Board>(new Board());
	b->init();
	return b;
}
