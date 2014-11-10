#ifndef SIMPLEPHYSICS_H
#define SIMPLEPHYSICS_H
#include <memory>
#include <set>
#include "body.h"

class SimplePhysics{
	SimplePhysics() = delete;
	static std::set<std::shared_ptr<StaticBody>> static_bodies;
	static std::set<std::shared_ptr<DynamicBody>> dynamic_bodies;
	friend class StaticBody;
	friend class DynamicBody;
	struct CollisionInfo{
		CollisionInfo(bool c, glm::vec2 n = glm::vec2(0.0,0.0)) : colliding(c), push(n){};
		bool colliding;
		glm::vec2 push;
	};
	static CollisionInfo CheckForCollision(std::shared_ptr<Body> b1, std::shared_ptr<Body> b2);
public:
	static void PerformIteration(float time_delta);
	static void RegisterSubtree(std::shared_ptr<Positionable> root);
	static void RegisterBody(std::shared_ptr<Body> b);
	static void RegisterDynamicBody(std::shared_ptr<DynamicBody> b);
	static void RegisterStaticBody(std::shared_ptr<StaticBody> b);
	static void UnRegisterBody(std::shared_ptr<Body> b);
	static void UnRegisterDynamicBody(std::shared_ptr<DynamicBody> b);
	static void UnRegisterStaticBody(std::shared_ptr<StaticBody> b);
	static void UnregisterAll();
};

#endif //SIMPLEPHYSICS_H
