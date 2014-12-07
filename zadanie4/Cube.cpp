#include "Cube.hpp"
#include "../engine/ModelBase.hpp"

#define rgb(x,y,z)	x/255.0, y/255.0, z/255.0, 1.0
#define CL rgb(150,150,150)
#define CD rgb(6, 25, 42)
#define CG rgb(80,150,80)
#define C_BLUE rgb(58, 92, 226)
#define C_RED rgb(223, 41, 31)
#define C_GREEN rgb(45, 199, 81)
#define x3(a) a,a,a
#define x3b(a,b,c) a,b,c,a,b,c,a,b,c
#define x12(a) a,a,a,a,a,a,a,a,a,a,a,a
#define x4(a,b,c) a,b,c,a,b,c,a,b,c,a,b,c

Cube::Cube(float scale){
	if(!ModelBase::GetInstance().HasModel("cube")){
		ModelBase::GetInstance().AddModelTriangles("cube",12,
		{ -1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, //
           1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f, //
           1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,  1.0f,-1.0f,-1.0f, //
           1.0f, 1.0f,-1.0f,  1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f, //
 		  -1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f,
           1.0f,-1.0f, 1.0f, -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,
          -1.0f, 1.0f, 1.0f, -1.0f,-1.0f, 1.0f,  1.0f,-1.0f, 1.0f,
           1.0f, 1.0f, 1.0f,  1.0f,-1.0f,-1.0f,  1.0f, 1.0f,-1.0f,
           1.0f,-1.0f,-1.0f,  1.0f, 1.0f, 1.0f,  1.0f,-1.0f, 1.0f,
           1.0f, 1.0f, 1.0f,  1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
           1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  1.0f,-1.0f, 1.0f},
	//	{ x3b(-1.0f, 0.0f, 0.0f), x3b(0.0f, 0.0f, -1.0f), x3b(0.0f, -1.0f, 0.0f),
    //      x3b(0.0f, 0.0f, -1.0f),},
		{ { CL,CD,CD,CL,CL,CL,CG,CG,CG,
            CL,CL,CL,CL,CD,CL,CG,CG,CG,
			CD,CD,CD,CD,CL,CL,CL,CD,CD,
			CD,CL,CL,CD,CL,CD,CD,CD,CD,},
          { x4(x3(C_GREEN),x3(C_RED),x3(C_BLUE)),},
          { x4(x3(C_RED),x3(C_RED),x3(C_RED)),},
			}
		);
	}
	model_id = "cube";
	SetScale(scale);
}
