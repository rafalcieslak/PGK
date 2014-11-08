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
			CheckForCollision(db, sb);

		}

		//TODO collisions with other dynamic bodies. Rigid physics!

	}

	// Apply linear velocities
	for(auto p : dynamic_bodies){
		glm::vec2 current_pos = p->GetPos();
		glm::vec2 new_pos = current_pos + p->linearVelocity*time_delta;
		p->SetPosAbsolute(new_pos);
	}
	std::cout << "end iteration" << std::endl;
}

inline glm::vec2 CheckCollisionCircleRectangle(std::shared_ptr<CollisionShape> circle_, std::shared_ptr<CollisionShape> rectangle_){
	auto circle    = std::dynamic_pointer_cast<CollisionShapeCircle>(circle_);
	auto rectangle = std::dynamic_pointer_cast<CollisionShapeRectangle>(rectangle_);
	if(!circle || !rectangle) return glm::vec2(0.0,0.0);

	std::cout << "Circle-rectangle" << std::endl;
	return glm::vec2(0.0,0.0);
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
