#include "Paddle.h"
#include "ModelBase.h"

Paddle::PaddleDrawable::PaddleDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("Paddle")){
		ModelBase::GetInstance().AddModel("Paddle",3,
			               {  1.0, 0.0, 2.0/3.0, -1.0/3.0, 0.0, 0.0,
							 -1.0, 0.0,-2.0/3.0, -1.0/3.0, 0.0, 0.0,
							  2.0/3.0, -1.0/3.0, -2.0/3.0, -1.0/3.0, 0.0, 0.0,},
			               { 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.6,0.0,
							 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.6,0.0,
							 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.6,0.0,});
	}
	model_id = "Paddle";
}

Paddle::PaddleBody::PaddleBody(){
	SetPosRelative(relative_pos);
	if(parent) parent->LinkChild(Positionable::shared_from_this());
}

void Paddle::PaddleBody::init(){
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(0.0, -1.0/6.0), glm::vec2(1.0,1.0/6.0));
}

Paddle::Paddle(){
	pd = std::make_shared<PaddleDrawable>();
	pb = std::make_shared<PaddleBody>();
	pb->SetScale(0.15);
	pb->init();
}

void Paddle::init(glm::vec2 pos){
	pb->LinkChild(pd);
	SetTop(pb);
	SetBottom(pd);
	SetPosRelative(pos);
}

std::shared_ptr<Paddle> Paddle::Create(glm::vec2 pos){
	auto b = std::shared_ptr<Paddle>(new Paddle());
	b->init(pos);
	return b;
}
