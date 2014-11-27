#ifndef BODY_H
#define BODY_H

#include "Node.hpp"
#include "Signal.hpp"
#include <vector>
#include <cmath>
#include <string>

/* These classes represent different shapes used for collision detection.
 * They only store shape paramenters.
 * The only place such instances are stored should be Body::shapes.
 * It is a Node, so it can be arranged in a tree, and is located at a relative position.
*/
struct CollisionShape : Node{
	// The bounding radius is used to quickly check whether two shapes can possibly collide.
	CollisionShape(float br) : bounding_radius(br) {}
	float GetBoundingRadius() {
		return bounding_radius * GetScale();
	}
private:
	float bounding_radius;
};
// This shape is a rectangle. It stores half of width and height as size.
struct CollisionShapeRectangle : public CollisionShape{
	CollisionShapeRectangle(glm::vec2 s) : CollisionShape(sqrt(s.x*s.x + s.y*s.y)), size(s) {  };
	glm::vec2 size;
};
// A circle is represented by it's radius.
struct CollisionShapeCircle : public CollisionShape{
	CollisionShapeCircle(float r) : CollisionShape(r), radius(r) {};
	float radius;
};
// This is a special shape, which is just a line segment, but from one side it deflects objects to it's sides.
struct CollisionShapePaddle : public CollisionShape{
	CollisionShapePaddle(float w, float d) : CollisionShape(w), width(w), deviation_ratio(d) {};
	float width;
	float deviation_ratio;
};

/* A Body is an elemental object for physics processing. It represents a singular rigid body.
 * A body may be subject to collisions and velocity.
 * It is a Node, so it can be arranged in a tree, and is located at a relative position.
 * It may be particularly useful to attach a Drawable as a child of a Body, so that it gets dragged
 * around as the Body (DynamicBody) moves.
*/
class Body : public Node{
protected:
	// Constructs a body. This shoul be considered an abstarct class,
	// please construct a StaticBody or a DynamicBody instead.
	Body() {};
	Body(std::string id_) : id(id_) {};
public:
	// The container for all shapes that define this body.
	std::vector<std::shared_ptr<CollisionShape>> shapes;
	// This signal is emmited when this body collides with another.
	Signal<std::shared_ptr<Body>> on_collision;
	// Bodies can be given names, to recognise what collided with them.
	std::string id;

	// This is a helper funcion for creating and applying new shapes.
	template<typename T, typename... Args> void AddNewCollisionShape(glm::vec2 offset, Args... args){
		auto n = std::make_shared<T>(args...);
		n->SetPosRelative(offset);
		LinkChild(n);
		shapes.push_back(n);
	}

	// Set this to false to disable collisions for this object.
	bool colliding = true;
};

// A static body is not subject to velocity. Two static bodies will never collide.
class StaticBody : public Body{
public:
	StaticBody() {};
	StaticBody(std::string id) : Body(id) {};
};

// A dynamic body moves around the field.
class DynamicBody : public Body{
public:
	DynamicBody() {};
	DynamicBody(std::string id) : Body(id) {};
	glm::vec2 linearVelocity;
};

#endif //BODY_H
