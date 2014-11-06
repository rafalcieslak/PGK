#include "Brick.h"
#include "ModelBase.h"

Brick::Brick(std::shared_ptr<Positionable> parent, glm::vec2 pos) : Drawable("", parent, pos){
	if(!ModelBase::GetInstance().HasModel("brick")){
		ModelBase::GetInstance().AddModel("brick",2, (float[]){ -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,
															    0.5, -0.5, -0.5, 0.5,  0.5, 0.5},
													(float[]){ 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,
													           1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,});
	}
	model_id = "brick";
}
