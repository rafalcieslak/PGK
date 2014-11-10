#ifndef BODY_H
#define BODY_H

#include "Positionable.hpp"
#include "Signal.hpp"
#include <vector>
#include <cmath>
#include <string>


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
protected:
	Body() {};
	Body(std::string id_) : id(id_) {};
public:
	std::vector<std::shared_ptr<CollisionShape>> shapes;
	Signal<std::shared_ptr<Body>> on_collision;
	std::string id;

	template<typename T, typename... Args> void AddNewCollisionShape(glm::vec2 offset, Args... args){
		auto n = std::make_shared<T>(args...);
		n->SetPosRelative(offset);
		LinkChild(n);
		shapes.push_back(n);
	}
	bool colliding = true;
};

class StaticBody : public Body{
public:
	StaticBody() {};
	StaticBody(std::string id) : Body(id) {};
};

class DynamicBody : public Body{
public:
	DynamicBody() {};
	DynamicBody(std::string id) : Body(id) {};
	glm::vec2 linearVelocity;
};

#endif //BODY_H
