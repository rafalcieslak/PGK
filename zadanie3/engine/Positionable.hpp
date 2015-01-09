#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <list>
#include <memory>
#include <algorithm>

glm::vec2 Rotate2dVector01(glm::vec2, float);

/* A positionable is the most basic object. It can represent a drawable element
 * a physical body, or anything that is represented by 2d position.
 * The positionables are arranged in tree-like structures. Each is aware of
 * its children and parent.
 * The basic properties of a Positionable are it's 2d position, scale and
 * rotation angle. These values are applied relative to it's parent, i.e.
 * as if the parent was the origin of coorinates. Because of that, if a parent
 * is moved/rotated, the children will also be affected, which allows to organize
 * the structure basing on semantical hierarchy.
*/
class Positionable : public std::enable_shared_from_this<Positionable>{
protected:
	// A link to the parent. It's a weak_ptr, to break cyclic references.
	std::weak_ptr<Positionable> parent;
	// 2D position of this Positionable - relative to parent.
	glm::vec2 relative_pos;
	// Scale and angle of this Positionable.
	float relative_scale = 1.0;
	float relative_angle = 0.0;
	// Function returns position (as xy), scale (as z) and angle (as w) in
	// ABSOLUTE coordinates. It traverses to the root node and applies all
	// transformations.
	glm::vec4 GetPosScaleAngle() const;
	// If false, the subtree of this node will be skipped in processing.
	bool active = true;
	virtual void SetParent(std::shared_ptr<Positionable>);
public:
	Positionable() {};
	Positionable(glm::vec2 relative_pos);
	// The list of children nodes.
	std::list<std::shared_ptr<Positionable>> children;

	// Returns the absolute position.
	glm::vec2 GetPos() const;
	// Returns the absolute scale.
	float GetScale() const;
	// Returns the absolute angle.
	float GetAngle() const;
	// Returns whether this node should be processed.
	bool GetActiveAbsolute();
	// Returns the relative position (r/w).
	glm::vec2& GetPosRelative();
	// Returns the relative scale (r/w).
	float& GetScaleRelative();
	// Returns activity state of this node (r/w).
	bool& GetActiveRelative();
	// Sets relative position.
	void SetPosRelative(glm::vec2);
	// Sets position in absolute coordinates. Useful for physics calculations,
	// as they are performed in absolute coordinates exclusivelly.
	void SetPosAbsolute(glm::vec2);
	// Setters for relative parematers.
	void SetScale(float);
	void SetAngle(float);
	void SetActive(bool);
	// Creates a parent-children relationship. Adds a child node, and set's
	// its parent to this node.
	virtual void LinkChild(std::shared_ptr<Positionable>);
	// Cleanup procedure. Removes patent-child relationship of this node and
	// it's parent. This may cause shared_ptrs to zero their use count,
	// which in turn would destruct all this subtree. Useful for removing
	// parts of a tree.
	void DetachFromParent();
	// A helper function that spawns a new Positionable managed by a shared_ptr.
	static std::shared_ptr<Positionable> Create(glm::vec2 pos = glm::vec2(0.0,0.0));
};

/* A positionable group is a helper node that stores multiple positionables
 * as if they were just one node. Elements that are made up of several
 * positionables are adviesed to derive from this class, it makes it easier
 * to maintains such tree if some nodes are grouped (like ball's body and drawable).
*/
class PositionableGroup : public Positionable{
	// The pointer to the root of this group
	std::shared_ptr<Positionable> top;
	// Where child nodes for this group should be attached.
	std::shared_ptr<Positionable> bottom;
public:
	// Tell the group where it's own inner root is.
	void SetTop(std::shared_ptr<Positionable> top);
	// Tell the group where it's children should be attached.
	void SetBottom(std::shared_ptr<Positionable> bottom);
	// A custom version of LinkChild, that attaches children to the node
	// designated by SetBottom.
	virtual void LinkChild(std::shared_ptr<Positionable>);
};

#endif //POSITIONABLE_H
