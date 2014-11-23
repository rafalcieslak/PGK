#include "Paddle.hpp"
#include "../engine-old/ModelBase.hpp"

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
Paddle::BounceHighlight::BounceHighlight() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("Paddle-bounce")){
		ModelBase::GetInstance().AddModelTriangles("Paddle-bounce",1,
		{  1.0, 0.0, 0.0, -1.0/10.0, -1.0, 0.0,},
		{ 0.0,1.0,0.2,1.0, 0.0,1.0,0.2,1.0, 0.0,1.0,0.2,1.0,});
	}
	model_id = "Paddle-bounce";
}

Paddle::PaddleBody::PaddleBody(){

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
	bh = std::make_shared<BounceHighlight>();
	pb->init();
}

void Paddle::init(glm::vec2 pos, float size){
	pb->LinkChild(pd);
	pb->LinkChild(bh);
	pb->SetScale(size);
	SetTop(pb);
	SetBottom(pd);
	SetPosRelative(pos);
	bh->SetActive(false);
	// When the ball touches the paddle, animate the highlight.
	pb->on_collision.Subscribe( [this](std::shared_ptr<Body> body){
		if(body->id == "ball"){
			this->bh->SetActive(true);
			this->bh->StartAnimation(1,0.15);
		}
	});
	// When animation is completed, disable the highlight.
	bh->on_animation_finished.Subscribe( [this](int){
		this->bh->SetActive(false);
	});
}

std::shared_ptr<Paddle> Paddle::Create(glm::vec2 pos, float size){
	auto b = std::shared_ptr<Paddle>(new Paddle());
	b->init(pos, size);
	return b;
}
