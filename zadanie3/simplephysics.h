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
public:
	static void PerformIteration(float time_delta);
};

#endif //SIMPLEPHYSICS_H
