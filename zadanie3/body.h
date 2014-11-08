#ifndef BODY_H
#define BODY_H

#include "Positionable.h"
#include <vector>

class Body : public Positionable{

public:
	typedef enum{
		BODY_TYPE_STATIC,
		BODY_TYPE_DYNAMIC
	} BodyType;
	virtual BodyType GetBodyType() const = 0;
};

class StaticBody : public Body{
public:
	StaticBody();
	~StaticBody();
	virtual BodyType GetBodyType() const {return Body::BODY_TYPE_STATIC;}
	static std::vector<StaticBody *> static_bodies;
};

class DynamicBody : public Body{
public:
	DynamicBody();
	~DynamicBody();
	glm::vec2 linearVelocity;
	virtual BodyType GetBodyType() const {return Body::BODY_TYPE_DYNAMIC;}
	static std::vector<DynamicBody *> dynamic_bodies;
};

#endif //BODY_H
