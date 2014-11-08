#ifndef SIMPLEPHYSICS_H
#define SIMPLEPHYSICS_H
#include <memory>
#include "body.h"

class SimplePhysics{
	SimplePhysics() = delete;
	static std::vector<StaticBody*> static_bodies;
	static std::vector<DynamicBody*> dynamic_bodies;
	friend class StaticBody;
	friend class DynamicBody;
	struct CollisionInfo{
		CollisionInfo(bool c, glm::vec2 n = glm::vec2(0.0,0.0)) : colliding(c), normal(n){};
		bool colliding;
		glm::vec2 normal;
	};
	static CollisionInfo CheckForCollision(Body* b1, Body* b2);
public:
	static void PerformIteration(float time_delta);
};

#endif //SIMPLEPHYSICS_H
