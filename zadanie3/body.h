#ifndef BODY_H
#define BODY_H

#include "Positionable.h"
#include <vector>
#include <cmath>


struct CollisionShape : Positionable{
	CollisionShape(float br) : bounding_radius(br) {}
	float GetBoundingRadius() {
		return bounding_radius * GetScale();
	}
private:
	float bounding_radius;
};

struct CollisionShapeRectangle : public CollisionShape{
	CollisionShapeRectangle(glm::vec2 s, float angle = 0.0) : CollisionShape(sqrt(s.x*s.x + s.y*s.y)), size(s) { SetAngle(angle); };
	glm::vec2 size;
};
struct CollisionShapeCircle : public CollisionShape{
	CollisionShapeCircle(float r) : CollisionShape(r), radius(r) {};
	float radius;
};
struct CollisionShapePaddle : public CollisionShape{
	CollisionShapePaddle(float w, float d) : CollisionShape(w), width(w), deviation_ratio(d) {};
	float width;
	float deviation_ratio;
};


class Body : public Positionable{
public:
	typedef enum{
		BODY_TYPE_STATIC,
		BODY_TYPE_DYNAMIC
	} BodyType;
	virtual BodyType GetBodyType() const = 0;
	template<typename T, typename... Args> void AddNewCollisionShape(glm::vec2 offset, Args... args){
		auto n = std::make_shared<T>(args...);
		n->SetPosRelative(offset);
		LinkChild(n);
		shapes.push_back(n);
	}
	std::vector<std::shared_ptr<CollisionShape>> shapes;
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
