#ifndef BODY_H
#define BODY_H

#include "Positionable.h"

class Body : public Positionable{

public:
	typedef enum{
		BODY_TYPE_STATIC,
		BODY_TYPE_DYNAMIC
	} BodyType;
	virtual BodyType GetBodyType() = 0;
};

class StaticBody : public Body{
public:
	virtual BodyType GetBodyType() {return Body::BODY_TYPE_STATIC;}
};

class DynamicBody : public Body{
public:
	glm::vec2 linearSpeed;
	virtual BodyType GetBodyType() {return Body::BODY_TYPE_DYNAMIC;}
};

#endif //BODY_H
