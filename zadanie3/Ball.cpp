#include "Ball.hpp"
#include "../engine/ModelBase.hpp"
#include <cmath>

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
std::array<float,BALL_PTS*12> prepare_ball_colors(){
	std::array<float,BALL_PTS*12> p;
	for(int i = 0; i < BALL_PTS; i++){
		p[i*12 + 0] = 0.7;
		p[i*12 + 1] = 0.2;
		p[i*12 + 2] = 0.0;
		p[i*12 + 3] = 1.0;
		p[i*12 + 4] = 1.0;
		p[i*12 + 5] = 0.7;
		p[i*12 + 6] = 0.7;
		p[i*12 + 7] = 1.0;
		p[i*12 + 8] = 0.7;
		p[i*12 + 9] = 0.2;
		p[i*12 +10] = 0.0;
		p[i*12 +11] = 1.0;
	}
	return p;
}

std::array<float,BALL_PTS*6> Ball::BallDrawable::ball_vertices = prepare_ball_vertices();
std::array<float,BALL_PTS*12> Ball::BallDrawable::ball_colors = prepare_ball_colors();

Ball::BallDrawable::BallDrawable() : Drawable(""){
	if(!ModelBase::GetInstance().HasModel("ball")){
		ModelBase::GetInstance().AddModelTriangles("ball",BALL_PTS, ball_vertices.data(), ball_colors.data());
	}
	model_id = "ball";
}

Ball::BallBody::BallBody() : DynamicBody("ball"){
	
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
