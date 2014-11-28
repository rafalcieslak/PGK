#include "Plane.hpp"
#include "../engine/ModelBase.hpp"

#define rgb(x,y,z)	x/255.0, y/255.0, z/255.0, 1.0
#define C_GRAY rgb(140, 140, 140)
#define x3(a) a,a,a

Plane::Plane(float scale){
	if(!ModelBase::GetInstance().HasModel("plane")){
		ModelBase::GetInstance().AddModelTriangles("plane",2,
		{ -1.0f, -1.0f, 0.0f,  -1.0f, 1.0f, 0.0f,  1.0f, -1.0f, 0.0f,
           1.0f,  1.0f, 0.0f,  1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f },
		{ x3(C_GRAY),x3(C_GRAY) }
		);
	}
	model_id = "plane";
	SetScale(scale);
}
