#include "SimplePhysics.hpp"
#include <iostream>
#include <memory>


/* Temporarily disabled
std::vector<std::shared_ptr< StaticBody>> SimplePhysics::static_bodies;
std::vector<std::shared_ptr<DynamicBody>> SimplePhysics::dynamic_bodies;

void SimplePhysics::PerformIteration(float time_delta){

	// Detect collisions
	///std::cout << "size = " << dynamic_bodies.size() << std::endl;
	for(std::shared_ptr<DynamicBody> db : dynamic_bodies){ // Only dynamic bodies can collide
		if(!db->GetActiveAbsolute() || !db->colliding) continue;

		// Check for collisions with static bodies
		for(std::shared_ptr<StaticBody> sb : static_bodies){
			if(!sb->GetActiveAbsolute() || !sb->colliding) continue;
			CollisionInfo ci = CheckForCollision(db, sb);
			if(ci.colliding){
				// Inform about collision
				db->on_collision.Happen(sb);
				sb->on_collision.Happen(db);
				// Apply push vector
				//std::cout << "PUSH " << ci.push.x << " " << ci.push.y << std::endl;
				db->SetPosAbsolute( db->GetPos() + ci.push );
				// Bounce velocity
				db->linearVelocity = glm::reflect( db->linearVelocity, glm::normalize(ci.push) );
			}

		}

		//TODO collisions with other dynamic bodies. Rigid physics!

	}

	// Apply linear velocities
	for(auto p : dynamic_bodies){
		glm::vec2 current_pos = p->GetPos();
		glm::vec2 new_pos = current_pos + p->linearVelocity*time_delta;
		p->SetPosAbsolute(new_pos);
	}
}

void SimplePhysics::RegisterSubtree(std::shared_ptr<Node> root){
	auto body = std::dynamic_pointer_cast< Body>(root);
	if(body) RegisterBody(body);
	for(auto ch : root->children) RegisterSubtree(ch);
}

void SimplePhysics::RegisterBody(std::shared_ptr<Body> b){
	auto sb = std::dynamic_pointer_cast< StaticBody>(b);
	auto db = std::dynamic_pointer_cast<DynamicBody>(b);
	if(sb) RegisterStaticBody(sb);
	else if(db) RegisterDynamicBody(db);
	else{
		std::cout << "Tryging to gerister a body which is of neither type!" << std::endl;
	};
}
void SimplePhysics::RegisterStaticBody(std::shared_ptr<StaticBody> b){
	if(std::find(static_bodies.begin(), static_bodies.end(), b) == static_bodies.end()) static_bodies.push_back(b);
}
void SimplePhysics::RegisterDynamicBody(std::shared_ptr<DynamicBody> b){
	if(std::find(dynamic_bodies.begin(), dynamic_bodies.end(), b) == dynamic_bodies.end()) dynamic_bodies.push_back(b);
}
void SimplePhysics::UnRegisterSubtree(std::shared_ptr<Node> root){
	auto body = std::dynamic_pointer_cast< Body>(root);
	if(body) UnRegisterBody(body);
	for(auto ch : root->children) UnRegisterSubtree(ch);
}


void SimplePhysics::UnRegisterBody(std::shared_ptr<Body> b){
	auto sb = std::dynamic_pointer_cast< StaticBody>(b);
	auto db = std::dynamic_pointer_cast<DynamicBody>(b);
	if(sb) UnRegisterStaticBody(sb);
	else if(db) UnRegisterDynamicBody(db);
	else{
		std::cout << "Tryging to ungerister a body which is of neither type!" << std::endl;
	};
}
void SimplePhysics::UnRegisterStaticBody(std::shared_ptr<StaticBody> b){
	auto it = std::find(static_bodies.begin(), static_bodies.end(), b);
	if(it != static_bodies.end()) static_bodies.erase(it);
}
void SimplePhysics::UnRegisterDynamicBody(std::shared_ptr<DynamicBody> b){
	auto it = std::find(dynamic_bodies.begin(), dynamic_bodies.end(), b);
	if(it != dynamic_bodies.end()) dynamic_bodies.erase(it);
}
void SimplePhysics::UnregisterAll(){
	dynamic_bodies.clear();
	static_bodies.clear();
}

// returns MPV for the circle
glm::vec2 CheckCollisionCircleRectangle(std::shared_ptr<CollisionShape> circle_, std::shared_ptr<CollisionShape> rectangle_){
	auto circle    = std::dynamic_pointer_cast<CollisionShapeCircle>(circle_);
	auto rectangle = std::dynamic_pointer_cast<CollisionShapeRectangle>(rectangle_);
	if(!circle || !rectangle) return glm::vec2(0.0,0.0);


	glm::vec2 trans = -1.0f* rectangle->GetPos();
	float angle = rectangle->GetAngle();
	glm::mat2 rot_mat = glm::mat2(glm::cos(angle*2.0*M_PI), glm::sin(angle*-2.0*M_PI), glm::sin(angle*2.0*M_PI), glm::cos(angle*2.0*M_PI));
	glm::mat2 rot_mat_r = glm::mat2(glm::cos(angle*2.0*M_PI), glm::sin(angle*2.0*M_PI), glm::sin(angle*-2.0*M_PI), glm::cos(angle*2.0*M_PI));
	glm::vec2 circle_pos = rot_mat_r * (circle->GetPos() + trans);
	float w_2 = rectangle->size.x * rectangle->GetScale();
	float h_2 = rectangle->size.y * rectangle->GetScale();
	float r = circle->radius * circle->GetScale();

	// to X axix
	float xpush = 1.0;
	float ypush = 1.0;
	const float rx1 = -w_2, rx2 = w_2;
	const float cx1 = circle_pos.x - r, cx2 = circle_pos.x + r;
	const float ry1 = -h_2, ry2 = h_2;
	const float cy1 = circle_pos.y - r, cy2 = circle_pos.y + r;
	if(cx2 < rx1 || cx1 > rx2 || cy2 < ry1 || cy1 > ry2) return glm::vec2(0.0,0.0);

	if(cx1<rx1 && cx2<rx2) xpush = rx1-cx2;
	else if(cx1>rx1 && cx2>rx2) xpush = rx2-cx1;
	else if(cx1>rx1 && cx2<rx2) {
		if (circle_pos.x > 0) xpush = rx2 - cx1;
		else xpush = rx1 - cx2;
	}else if(cx1<rx1 && cx2>rx2){
		if (circle_pos.x > 0) xpush = rx2 - cx1;
		else xpush = rx1 - cx2;
	}
	// to Y axix
	if(cy1<ry1 && cy2<ry2) ypush = ry1-cy2;
	else if(cy1>ry1 && cy2>ry2) ypush = ry2-cy1;
	else if(cy1>ry1 && cy2<ry2) {
		if (circle_pos.x > 0) ypush = ry2 - cy1;
		else ypush = ry1 - cy2;
	}else if(cy1<ry1 && cy2>ry2){
		if (circle_pos.x > 0) ypush = ry2 - cy1;
		else ypush = ry1 - cy2;
	}
	glm::vec2 res;
	if(glm::abs(xpush) < 0.0000001 || glm::abs(ypush) < 0.0000001) res = glm::vec2(0.0,0.0);
	else if(glm::abs(xpush) < glm::abs(ypush)) res = glm::vec2(xpush,0.0);
	else res = glm::vec2(0.0,ypush);

	return rot_mat * res;
}

glm::vec2 CheckCollisionCirclePaddle(std::shared_ptr<CollisionShape> circle_, std::shared_ptr<CollisionShape> paddle_){
	auto circle = std::dynamic_pointer_cast<CollisionShapeCircle>(circle_);
	auto paddle = std::dynamic_pointer_cast<CollisionShapePaddle>(paddle_);
	if(!circle || !paddle) return glm::vec2(0.0,0.0);

	glm::vec2 trans = -1.0f* paddle->GetPos();
	float angle = paddle->GetAngle();
	glm::mat2 rot_mat = glm::mat2(glm::cos(angle*2.0*M_PI), glm::sin(angle*-2.0*M_PI), glm::sin(angle*2.0*M_PI), glm::cos(angle*2.0*M_PI));
	glm::mat2 rot_mat_r = glm::mat2(glm::cos(angle*2.0*M_PI), glm::sin(angle*2.0*M_PI), glm::sin(angle*-2.0*M_PI), glm::cos(angle*2.0*M_PI));
	glm::vec2 circle_pos = rot_mat_r * (circle->GetPos() + trans);

	float w = paddle->width * paddle->GetScale();
	float r = circle->radius * circle->GetScale();
	float cy1 = circle_pos.y-r, cy2 = circle_pos.y+r;
	float cx1 = circle_pos.x-r, cx2 = circle_pos.x+r;

	if(cy1 > 0.0 || cy2 < 0.0 || cx2 < -w || cx1 > w) return glm::vec2(0.0,0.0);
	glm::vec2 result;
	float hit_pt = circle_pos.x / w;
	if(hit_pt > 1.0){ // right paddle edge hit
		glm::vec2 direction = circle_pos - glm::vec2(w,0.0);
		float dist = glm::max(0.0f, glm::length(direction)-r);
		result = glm::normalize(direction)*dist;
	}else if(hit_pt < -1.0){ // left padle edge hit
		glm::vec2 direction = circle_pos - glm::vec2(-w,0.0);
		float dist = glm::max(0.0f, glm::length(direction)-r);
		result = glm::normalize(direction)*dist;
	}
	else{ // main paddle area hit
		if(circle_pos.y <= 0.001){
			// paddle hit from bottom
			result = glm::vec2(0.0,-cy2);
		}else{
			float pushy = -cy1;
			float pushx = hit_pt * pushy * paddle->deviation_ratio;
			//std::cout << "pushx " << pushx << ", hit_pt " << hit_pt << std::endl;
			result = glm::vec2(pushx,pushy);
		}
	}

	return rot_mat * result;

}

#define CT(type,ptr) (std::dynamic_pointer_cast<type>(ptr)!=nullptr)

SimplePhysics::CollisionInfo SimplePhysics::CheckForCollision(std::shared_ptr<Body> b1, std::shared_ptr<Body> b2){
	glm::vec2 normal_sum = glm::vec2(0.0,0.0);
	for(auto shape1 : b1->shapes){
		for(auto shape2 : b2->shapes){
			float distance = glm::distance(shape1->GetPos(), shape2->GetPos());
			//std::cout << distance << " " << shape1->GetBoundingRadius() << " " << shape2->GetBoundingRadius() << std::endl;
			if(distance > shape1->GetBoundingRadius() + shape2->GetBoundingRadius())
				continue; //too far away
			if(CT(CollisionShapeCircle,shape1) && CT(CollisionShapeRectangle,shape2)) normal_sum += CheckCollisionCircleRectangle(shape1,shape2);
			else if(CT(CollisionShapeCircle,shape2) && CT(CollisionShapeRectangle,shape1)) normal_sum += CheckCollisionCircleRectangle(shape2,shape1);
			else if(CT(CollisionShapeCircle,shape1) && CT(CollisionShapePaddle,shape2)) normal_sum += CheckCollisionCirclePaddle(shape1,shape2);
			else if(CT(CollisionShapeCircle,shape2) && CT(CollisionShapePaddle,shape1)) normal_sum += CheckCollisionCirclePaddle(shape2,shape1);
			else{
				std::cout << "This collision type is not implemented!" << std::endl;
			}
		}
	}

	if(glm::length(normal_sum) < 0.00001) return CollisionInfo(false); // no collision
	else return CollisionInfo(true,normal_sum);
}

*/
