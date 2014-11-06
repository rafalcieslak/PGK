#include "simplephysics.h"
std::vector<StaticBody*> SimplePhysics::static_bodies;
std::vector<DynamicBody*> SimplePhysics::dynamic_bodies;

void SimplePhysics::PerformIteration(float time_delta){


	// Apply linear velocities
	for(auto p : dynamic_bodies){
		p->PosRelative() += p->linearVelocity*time_delta;
	}
}
