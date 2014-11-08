#include "Brick.h"
#include "ModelBase.h"

Brick::BrickDrawable::BrickDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("brick")){
		ModelBase::GetInstance().AddModel("brick",2,
			(float[]){ -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,
			            0.5, -0.5, -0.5, 0.5,  0.5, 0.5},
			(float[]){ 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,
			           1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,});
	}
	model_id = "brick";
}

Brick::BrickBody::BrickBody(){
	SetPosRelative(relative_pos);
	if(parent) parent->LinkChild(Positionable::shared_from_this());
}

Brick::Brick(){
	bd = std::make_shared<BrickDrawable>();
	bb = std::make_shared<BrickBody>();
}

void Brick::init(glm::vec2 pos){
	bb->LinkChild(bd);
	SetTop(bb);
	SetBottom(bd);
	SetPosRelative(pos);
}

std::shared_ptr<Brick> Brick::Create(glm::vec2 pos){
	auto b = std::shared_ptr<Brick>(new Brick());
	b->init(pos);
	return b;
}
