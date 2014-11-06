#ifndef BODY_H
#define BODY_H

class Body{
	glm::vec2 pos;

public:
	typedef enum{
		BODY_TYPE_STATIC,
		BODY_TYPE_DYNAMIC
	} BodyType;
	BodyType GetBodyType() = 0;
};

class StaticBody : public Body{
public:
	BodyType GetBodyType() {return Body::BODY_TYPE_STATIC;}
}

class DynamicBody : public Body{
	glm::vec2 linearSpeed;
public:
	BodyType GetBodyType() {return Body::BODY_TYPE_DYNAMIC;}
}

#endif //BODY_H
