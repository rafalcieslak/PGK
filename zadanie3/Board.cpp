#include "Board.h"
#include "ModelBase.h"
#include <iostream>

Board::BoardDrawable::BoardDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("Board")){
		ModelBase::GetInstance().AddModel("Board", 6,
		               { 1.0, 1.0, 1.0, 0.0, 0.0, SQRT3,
		                -1.0, 1.0,-1.0, 0.0,-0.0, SQRT3,
		       	  	     0.0, SQRT3, 0.5, SQRT3/2.0, -0.5, SQRT3/2.0,
						 1.0, 0.0, 1.0,-1.0, 0.0, -SQRT3,
						-1.0, 0.0,-1.0,-1.0,-0.0, -SQRT3,
						 0.0,-SQRT3, 0.5,-SQRT3/2.0, -0.5,-SQRT3/2.0,},
		               { 0.0,0.2,0.9, 0.0,0.2,0.9, 0.0,0.2,0.9,
		                 0.0,0.2,0.9, 0.0,0.2,0.9, 0.0,0.2,0.9,
						 0.0,0.2,0.9, 0.0,0.2,0.9, 0.0,0.2,0.9,
						 0.0,0.2,0.9, 0.0,0.2,0.9, 0.0,0.2,0.9,
						 0.0,0.2,0.9, 0.0,0.2,0.9, 0.0,0.2,0.9,
						 0.0,0.05,0.2, 0.0,0.05,0.2, 0.0,0.05,0.2,});
	}
	model_id = "Board";
}

Board::BoardBody::BoardBody(){
	SetPosRelative(relative_pos);
	//if(parent) parent->LinkChild(Positionable::shared_from_this());
}

void Board::BoardBody::init(){
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(0.0, 1.0), glm::vec2(0.5,1.0-SQRT3/2.0));
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(1.5, SQRT3/2.0), glm::vec2(0.5,SQRT3/2.0), 1.0/6.0);
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(-1.5, SQRT3/2.0), glm::vec2(0.5,SQRT3/2.0), -1.0/6.0);
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(1.5, -SQRT3/2.0), glm::vec2(0.8,SQRT3/2.0), -1.0/6.0);
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(-1.5, -SQRT3/2.0), glm::vec2(0.8,SQRT3/2.0), 1.0/6.0);
}

Board::Board(){
	bd = std::make_shared<BoardDrawable>();
	body = std::make_shared<BoardBody>();
	body->init();
}

void Board::init(){
	Positionable::LinkChild(body);
	body->LinkChild(bd);
	SetTop(body);
	SetBottom(bd);
}

std::shared_ptr<Board> Board::Create(){
	auto b = std::shared_ptr<Board>(new Board());
	b->init();
	return b;
}
