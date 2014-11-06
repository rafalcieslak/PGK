#include "Brick.h"
#include "ModelBase.h"

BrickDrawable::BrickDrawable(glm::vec2 pos) : Drawable("", pos){
	if(!ModelBase::GetInstance().HasModel("brick")){
		ModelBase::GetInstance().AddModel("brick",2, (float[]){ -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,
															    0.5, -0.5, -0.5, 0.5,  0.5, 0.5},
													(float[]){ 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,
													           1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,});
	}
	model_id = "brick";
}

BrickBody::BrickBody(glm::vec2 relative_pos){
	SetPosRelative(relative_pos);
	if(parent) parent->LinkChild(Positionable::shared_from_this());
}
