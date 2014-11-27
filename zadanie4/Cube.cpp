#include "Cube.hpp"
#include "../engine/ModelBase.hpp"

#define rgb(x,y,z)	x/255.0, y/255.0, z/255.0, 1.0
#define C_BLUE rgb(58, 92, 226)
#define C_RED rgb(223, 41, 31)
#define x3(a) a,a,a
#define x12(a) a,a,a,a,a,a,a,a,a,a,a,a
#define x4(a,b,c) a,b,c,a,b,c,a,b,c,a,b,c

Cube::Cube(float scale){
	if(!ModelBase::GetInstance().HasModel("cube")){
		ModelBase::GetInstance().AddModelTriangles("cube",12,
		{ -1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
           1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
           1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,  1.0f,-1.0f,-1.0f,
           1.0f, 1.0f,-1.0f,  1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f,
 		  -1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f,
           1.0f,-1.0f, 1.0f, -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,
          -1.0f, 1.0f, 1.0f, -1.0f,-1.0f, 1.0f,  1.0f,-1.0f, 1.0f,
           1.0f, 1.0f, 1.0f,  1.0f,-1.0f,-1.0f,  1.0f, 1.0f,-1.0f,
           1.0f,-1.0f,-1.0f,  1.0f, 1.0f, 1.0f,  1.0f,-1.0f, 1.0f,
           1.0f, 1.0f, 1.0f,  1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
           1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  1.0f,-1.0f, 1.0f},
		{ { x4(x3(C_BLUE),x3(C_BLUE),x3(C_RED)),},
          { x4(x3(C_RED),x3(C_RED),x3(C_BLUE)),},
			}
		);
	}
	model_id = "cube";
	SetScale(scale);
}