#include "Brick.hpp"
#include "../engine/ModelBase.hpp"

#define rgb(x,y,z)	x/255.0, y/255.0, z/255.0
#define C_BLUE_A rgb(58, 92, 226)
#define C_BLUE_B rgb(133, 156, 248)
#define C_BLUE_D rgb(42, 58, 120)
#define C_RED_A rgb(223, 41, 31)
#define C_RED_B rgb(247, 116, 109)
#define C_RED_D rgb(105, 25, 21)
#define C_GREEN_A rgb(45, 199, 81)
#define C_GREEN_B rgb(91, 239, 126)
#define C_GREEN_D rgb(20, 86, 35)

Brick::BrickDrawable::BrickDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("brick")){
		ModelBase::GetInstance().AddModelTriangles("brick",4,
		{ -1.0, -0.5,  1.0, -0.5, -1.0, 0.5,
		   1.0, -0.5, -1.0,  0.5,  1.0, 0.5,
		  -0.8, -0.4,  0.8, -0.4, -0.8, 0.4,
		   0.8, -0.4, -0.8,  0.4,  0.8, 0.4},
       {{ C_BLUE_D, C_BLUE_D, C_BLUE_D,
	      C_BLUE_D, C_BLUE_D, C_BLUE_A,
		  C_BLUE_D, C_BLUE_A, C_BLUE_A,
		  C_BLUE_A, C_BLUE_A, C_BLUE_B,},
		{ C_RED_D, C_RED_D, C_RED_D,
		  C_RED_D, C_RED_D, C_RED_A,
          C_RED_D, C_RED_A, C_RED_A,
	      C_RED_A, C_RED_A, C_RED_B,},
		{ C_GREEN_D, C_GREEN_D, C_GREEN_D,
		  C_GREEN_D, C_GREEN_D, C_GREEN_A,
		  C_GREEN_D, C_GREEN_A, C_GREEN_A,
		  C_GREEN_A, C_GREEN_A, C_GREEN_B,},});
	}
	model_id = "brick";
}

Brick::BrickBody::BrickBody(){
	SetPosRelative(relative_pos);
	//if(parent) parent->LinkChild(Positionable::shared_from_this());
}

void Brick::BrickBody::init(){
	AddNewCollisionShape<CollisionShapeRectangle>(glm::vec2(0.0, 0.0), glm::vec2(1.0,0.5));
}

Brick::Brick(float size){
	bd = std::make_shared<BrickDrawable>();
	bb = std::make_shared<BrickBody>();
	bb->init();
	bb->SetScale(size);
	bb->on_collision.Subscribe( [this](std::shared_ptr<Body> body){
		if(body->id == "ball"){
			this->on_ball_collision.Happen();
			this->Break();
		}
	});
}

void Brick::init(glm::vec2 pos, unsigned int variant){
	bb->LinkChild(bd);
	SetTop(bb);
	SetBottom(bd);
	SetPosRelative(pos);
	bd->variant = variant;
}

std::shared_ptr<Brick> Brick::Create(glm::vec2 pos, unsigned int variant, float size){
	auto b = std::shared_ptr<Brick>(new Brick(size));
	b->init(pos, variant);
	return b;
}

void Brick::Break(){
	bb->colliding = false;
	bd->StartAnimation(1,0.3);
	bd->on_animation_finished.Subscribe([this](int i){
		if(i == 0) this->SetActive(false);
	});
}
