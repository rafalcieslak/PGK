#ifndef SIMPLEPHYSICS_H
#define SIMPLEPHYSICS_H
#include <memory>
#include <set>
#include "Body.hpp"

/* This static class performs all calculations that simulate physics.
*/
class SimplePhysics{
	SimplePhysics() = delete;
	// Lists of bodies that need processing each iteration.
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
	// Triggers a single iteration of physics calculation, for a given time
	// slice.
	static void PerformIteration(float time_delta);
	// Functions for registering bodies in the physics engine.
	static void RegisterSubtree(std::shared_ptr<Positionable> root);
	static void RegisterBody(std::shared_ptr<Body> b);
	static void RegisterDynamicBody(std::shared_ptr<DynamicBody> b);
	static void RegisterStaticBody(std::shared_ptr<StaticBody> b);
	// Functions for unregistering bodies from the physics engine.
	static void UnRegisterBody(std::shared_ptr<Body> b);
	static void UnRegisterDynamicBody(std::shared_ptr<DynamicBody> b);
	static void UnRegisterStaticBody(std::shared_ptr<StaticBody> b);
	static void UnRegisterSubtree(std::shared_ptr<Positionable> root);
	static void UnregisterAll();
};

#endif //SIMPLEPHYSICS_H
