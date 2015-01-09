#include "Overlay.hpp"
#include "engine/ModelBase.hpp"

#define OVERLAY_COLOR 0.0,0.0,0.0,0.7

Overlay::Overlay() : Drawable("overlay"){
	if(!ModelBase::GetInstance().HasModel("overlay")){
		ModelBase::GetInstance().AddModelTriangles("overlay",2,
		   {1.0,1.0,-1.0,1.0,1.0,-1.0,
		    -1.0,1.0,1.0,-1.0,-1.0,-1.0},
		   {OVERLAY_COLOR,OVERLAY_COLOR,OVERLAY_COLOR,
		    OVERLAY_COLOR,OVERLAY_COLOR,OVERLAY_COLOR});
	}
	model_id = "overlay";
}

std::shared_ptr<Overlay> Overlay::Create(){
	return std::shared_ptr<Overlay>(new Overlay());
}
