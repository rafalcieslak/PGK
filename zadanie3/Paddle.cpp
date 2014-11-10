#include "Paddle.hpp"
#include "../engine/ModelBase.hpp"

#define SQRT2 1.41421356237

Paddle::PaddleDrawable::PaddleDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("Paddle")){
		ModelBase::GetInstance().AddModelTriangles("Paddle",3,
			               {  1.0, 0.0, 2.0/3.0, -1.0/3.0, 0.0, 0.0,
							 -1.0, 0.0,-2.0/3.0, -1.0/3.0, 0.0, 0.0,
							  2.0/3.0, -1.0/3.0, -2.0/3.0, -1.0/3.0, 0.0, 0.0,},
			               { 1.0,0.0,0.0,1.0, 1.0,0.0,0.0,1.0, 1.0,0.6,0.0,1.0,
							 1.0,0.0,0.0,1.0, 1.0,0.0,0.0,1.0, 1.0,0.6,0.0,1.0,
							 1.0,0.0,0.0,1.0, 1.0,0.0,0.0,1.0, 1.0,0.6,0.0,1.0,});
	}
	model_id = "Paddle";
}

Paddle::PaddleBody::PaddleBody(){
	SetPosRelative(relative_pos);
	//if(parent) parent->LinkChild(Positionable::shared_from_this());
}

void Paddle::PaddleBody::init(){
	// Upper side
	AddNewCollisionShape<CollisionShapePaddle>(glm::vec2(0.0, 0.0), 1.0, 0.3);
	// Bottom
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(0.0, -1.0/4.0), glm::vec2(2.0/3.0,1.0/12.0));
	// Sides
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2( 19.0/24.0, -1.0/6.0), glm::vec2(5.0*SQRT2/96.0, SQRT2/48.0),  1.0/8.0);
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(-19.0/24.0, -1.0/6.0), glm::vec2(5.0*SQRT2/96.0, SQRT2/48.0), -1.0/8.0);
}

Paddle::Paddle(){
	pd = std::make_shared<PaddleDrawable>();
	pb = std::make_shared<PaddleBody>();
	pb->init();
}

void Paddle::init(glm::vec2 pos, float size){
	pb->LinkChild(pd);
	pb->SetScale(size);
	SetTop(pb);
	SetBottom(pd);
	SetPosRelative(pos);
}

std::shared_ptr<Paddle> Paddle::Create(glm::vec2 pos, float size){
	auto b = std::shared_ptr<Paddle>(new Paddle());
	b->init(pos, size);
	return b;
}
