#include "body.h"
#include "simplephysics.h"

StaticBody::StaticBody(){
	SimplePhysics::static_bodies.emplace_back(this);
}

StaticBody::~StaticBody(){
	auto p = std::find(SimplePhysics::static_bodies.begin(), SimplePhysics::static_bodies.end(), this);
	if (p != SimplePhysics::static_bodies.end())
		SimplePhysics::static_bodies.erase(p);
}
DynamicBody::DynamicBody(){
	SimplePhysics::dynamic_bodies.emplace_back(this);
}

DynamicBody::~DynamicBody(){
	auto p = std::find(SimplePhysics::dynamic_bodies.begin(), SimplePhysics::dynamic_bodies.end(), this);
	if (p != SimplePhysics::dynamic_bodies.end())
		SimplePhysics::dynamic_bodies.erase(p);
}
