#include "Ball.h"
#include "ModelBase.h"
#include <array>
#include <cmath>
#include <iostream>

std::array<float,BALL_PTS*6> prepare_ball_vertices(){
	std::array<float,BALL_PTS*6> p;
	for(int i = 0; i < BALL_PTS; i++){
		p[i*6 + 0] = cos(i*2.0*M_PI/(float)BALL_PTS);
		p[i*6 + 1] = sin(i*2.0*M_PI/(float)BALL_PTS);
		p[i*6 + 2] = 0.5;
		p[i*6 + 3] = 0.5;
		p[i*6 + 4] = cos((i+1)*2.0*M_PI/(float)BALL_PTS);
		p[i*6 + 5] = sin((i+1)*2.0*M_PI/(float)BALL_PTS);
	}
	return p;
}
std::array<float,BALL_PTS*9> prepare_ball_colors(){
	std::array<float,BALL_PTS*9> p;
	for(int i = 0; i < BALL_PTS; i++){
		p[i*9 + 0] = 0.7;
		p[i*9 + 1] = 0.2;
		p[i*9 + 2] = 0.0;
		p[i*9 + 3] = 1.0;
		p[i*9 + 4] = 0.7;
		p[i*9 + 5] = 0.7;
		p[i*9 + 6] = 0.7;
		p[i*9 + 7] = 0.2;
		p[i*9 + 8] = 0.0;
	}
	return p;
}

std::array<float,BALL_PTS*6> Ball::BallDrawable::ball_vertices = prepare_ball_vertices();
std::array<float,BALL_PTS*9> Ball::BallDrawable::ball_colors = prepare_ball_colors();

Ball::BallDrawable::BallDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("ball")){
		ModelBase::GetInstance().AddModel("ball",BALL_PTS, ball_vertices.data(), ball_colors.data());
	}
	model_id = "ball";
}

Ball::BallBody::BallBody() : DynamicBody("ball"){
	SetPosRelative(relative_pos);
	//if(parent) parent->LinkChild(Positionable::shared_from_this());
}

void Ball::BallBody::init(){
	AddNewCollisionShape<CollisionShapeCircle>(glm::vec2(0.0, 0.0), 1.0);
}

Ball::Ball(){
	bd = std::make_shared<BallDrawable>();
	body = std::make_shared<BallBody>();
	body->init();
}

void Ball::init(glm::vec2 pos){
	Positionable::LinkChild(body);
	body->LinkChild(bd);
	SetTop(body);
	SetBottom(bd);
	SetPosRelative(pos);
}

std::shared_ptr<Ball> Ball::Create(glm::vec2 pos){
	auto b = std::shared_ptr<Ball>(new Ball());
	b->init(pos);
	return b;
}
