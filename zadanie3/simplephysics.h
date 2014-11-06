#ifndef SIMPLEPHYSICS_H
#define SIMPLEPHYSICS_H
#include <memory>
#include "body.h"

class SimplePhysics{
	std::vector<Body> static_bodies;
	std::vector<Body> dynamic_bodies;
public:
	void PerformIteration(float time_delta);
	void AddBody( std::shared_ptr<Body> body);
};

#endif //SIMPLEPHYSICS_H
