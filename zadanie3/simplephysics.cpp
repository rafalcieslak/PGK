#include "simplephysics.h"
#include <iostream>
std::vector<StaticBody*> SimplePhysics::static_bodies;
std::vector<DynamicBody*> SimplePhysics::dynamic_bodies;

void SimplePhysics::PerformIteration(float time_delta){


	// Apply linear velocities
	for(auto p : dynamic_bodies){
		glm::vec2 current_pos = p->GetPos();
		glm::vec2 new_pos = current_pos + p->linearVelocity*time_delta;
		p->SetPosAbsolute(new_pos);
	}
}
