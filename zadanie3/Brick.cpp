#include "Brick.h"
#include "ModelBase.h"

Brick::Brick(glm::vec2 pos) : Drawable("",pos){
	if(!ModelBase::GetInstance().HasModel("brick")){
		ModelBase::GetInstance().AddModel("brick",2, (float[]){ -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,
															    0.5, -0.5, -0.5, 0.5,  0.5, 0.5},
													(float[]){ 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,
													           1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,});
	}
	model_id = "brick";
}
