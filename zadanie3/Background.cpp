#include "Background.hpp"
#include "../engine/ModelBase.hpp"
#include <cmath>

#define SQRT3 1.73205080757

std::vector<float> prepare_tile_vertices(){
	std::vector<float> p(6*6);
	for(int i = 0; i < 6; i++){
		if(i%2){
			p[i*6 + 0] = cos(i*2.0*M_PI/(float)6);
			p[i*6 + 1] = sin(i*2.0*M_PI/(float)6);
			p[i*6 + 2] = -0.0;
			p[i*6 + 3] = -0.0;
			p[i*6 + 4] = cos((i+1)*2.0*M_PI/(float)6);
			p[i*6 + 5] = sin((i+1)*2.0*M_PI/(float)6);
		}else{
			p[i*6 + 4] = cos(i*2.0*M_PI/(float)6);
			p[i*6 + 5] = sin(i*2.0*M_PI/(float)6);
			p[i*6 + 2] = -0.0;
			p[i*6 + 3] = -0.0;
			p[i*6 + 0] = cos((i+1)*2.0*M_PI/(float)6);
			p[i*6 + 1] = sin((i+1)*2.0*M_PI/(float)6);
		}
	}
	return p;
}
std::vector<float> prepare_tile_l_vertices(){
	std::vector<float> p(6*4);
	for(int i = 0; i < 6; i++){
		p[i*4 + 0] = cos(i*2.0*M_PI/(float)6);
		p[i*4 + 1] = sin(i*2.0*M_PI/(float)6);
		p[i*4 + 2] = cos((i+1)*2.0*M_PI/(float)6);
		p[i*4 + 3] = sin((i+1)*2.0*M_PI/(float)6);
	}
	return p;
}


#define rgb(x,y,z)	x/255.0, y/255.0, z/255.0
#define C_DBLUE_A rgb(58, 92, 226)
#define C_DBLUE_B rgb(48, 108, 248)
#define C_DBLUE_D rgb(37, 50, 90)

float trig_colors[9] = {
	C_DBLUE_A,
	C_DBLUE_B,
	C_DBLUE_D,
};
std::vector<float> prepare_tile_colors(){
	std::vector<float> p(6*9);
	for(int i = 0; i < 6; i++)
		for (int k = 0; k < 9; k++)
			p[i*9 + k] = trig_colors[k];
	return p;
}
std::vector<float> prepare_tile_l_colors(){
	std::vector<float> p(6*6);
	for(int i = 0; i < 6; i++){
		p[i*6 + 0] = 0.0;
		p[i*6 + 1] = 0.0;
		p[i*6 + 2] = 0.7;
		p[i*6 + 3] = 0.0;
		p[i*6 + 4] = 0.0;
		p[i*6 + 5] = 0.7;
	}
	return p;
}


Background::BackgroundDrawableTile::BackgroundDrawableTile() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("background")){
		std::vector<float> tile_vert = prepare_tile_vertices();
		std::vector<float> tile_cols = prepare_tile_colors();
		ModelBase::GetInstance().AddModelTriangles("background",6, tile_vert.data(), tile_cols.data());
	}
	model_id = "background";
}
Background::BackgroundDrawableLines::BackgroundDrawableLines() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("background_l")){
		std::vector<float> tile_vert = prepare_tile_l_vertices();
		std::vector<float> tile_cols = prepare_tile_l_colors();
		ModelBase::GetInstance().AddModelLines("background_l",6, tile_vert.data(), tile_cols.data());
	}
	model_id = "background_l";
}

Background::Background(){
	root = Positionable::Create();

}

void Background::init(float a, int w, int h){
	SetTop(root);
	for(int y = -h; y < h; y++){
		int par = (y>0?y:-y)%2;
		float xoffset = par*1.5*a;
		for(int x = -w/2; x <= w/2 - par; x++){
			auto tile = std::make_shared<BackgroundDrawableTile>();
			tile->SetScale(a);
			tile->SetPosRelative(glm::vec2(3*a*x + xoffset, a*y*SQRT3*0.5));
			root->LinkChild(tile);
			auto tile_l = std::make_shared<BackgroundDrawableLines>();
			tile_l->SetScale(a);
			tile_l->SetPosRelative(glm::vec2(3*a*x + xoffset, a*y*SQRT3*0.5));
			root->LinkChild(tile_l);
			drawables_l.push_back(tile_l);
		}
	}
}

std::shared_ptr<Background> Background::Create(float a, int w, int h){
	auto b = std::shared_ptr<Background>(new Background());
	b->init(a, w, h);
	return b;
}
