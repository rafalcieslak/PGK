#include "simplephysics.h"
#include <iostream>
#include <memory>
std::vector<StaticBody*> SimplePhysics::static_bodies;
std::vector<DynamicBody*> SimplePhysics::dynamic_bodies;

void SimplePhysics::PerformIteration(float time_delta){

	// Detect collisions
	for(DynamicBody* db : dynamic_bodies){ // Only dynamic bodies can collide

		// Check for collisions with static bodies
		for(StaticBody* sb : static_bodies){
			CollisionInfo ci = CheckForCollision(db, sb);
			if(ci.colliding){
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
	//std::cout << "end iteration" << std::endl;
}

// returns MPV for the circle
inline glm::vec2 CheckCollisionCircleRectangle(std::shared_ptr<CollisionShape> circle_, std::shared_ptr<CollisionShape> rectangle_){
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

//	std::cout << circle_pos.x << " " << circle_pos.y << " " << std::endl;

	// to X axix
	float xpush = 1.0;
	float ypush = 1.0;
	const float rx1 = -w_2, rx2 = w_2;
	const float cx1 = circle_pos.x - r, cx2 = circle_pos.x + r;
	const float ry1 = -h_2, ry2 = h_2;
	const float cy1 = circle_pos.y - r, cy2 = circle_pos.y + r;
	if(cx2 < rx1 || cx1 > rx2) xpush = 0.0;
	if(cy2 < ry1 || cy1 > ry2) ypush = 0.0;
	if(glm::abs(xpush) < 0.0000001 || glm::abs(ypush) < 0.0000001) return glm::vec2(0.0,0.0);
	;
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

#define CT(type,ptr) std::dynamic_pointer_cast<type>(ptr)

SimplePhysics::CollisionInfo SimplePhysics::CheckForCollision(Body* b1, Body* b2){
	glm::vec2 normal_sum = glm::vec2(0.0,0.0);
	for(auto shape1 : b1->shapes){
		for(auto shape2 : b2->shapes){
			float distance = glm::distance(shape1->GetPos(), shape2->GetPos());
			//std::cout << distance << " " << shape1->GetBoundingRadius() << " " << shape2->GetBoundingRadius() << std::endl;
			if(distance > shape1->GetBoundingRadius() + shape2->GetBoundingRadius())
				continue; //too far away
			if(CT(CollisionShapeCircle,shape1) && CT(CollisionShapeRectangle,shape2)) normal_sum += CheckCollisionCircleRectangle(shape1,shape2);
			else if(CT(CollisionShapeCircle,shape2) && CT(CollisionShapeRectangle,shape1)) normal_sum += CheckCollisionCircleRectangle(shape2,shape1);
			else{
				std::cout << "This collision type is not implemented!" << std::endl;
			}
		}
	}

	if(glm::length(normal_sum) < 0.001) return CollisionInfo(false); // no collision
	else return CollisionInfo(true,normal_sum);
}
