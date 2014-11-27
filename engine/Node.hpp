#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <list>
#include <memory>
#include <algorithm>

glm::vec2 Rotate2dVector01(glm::vec2, float);

/* A Node is the most basic object. It can represent a drawable element
 * a physical body, or anything that is represented by 2d position.
 * The Nodes are arranged in tree-like structures. Each is aware of
 * its children and parent.
 * The basic properties of a Node are it's 2d position, scale and
 * rotation angle. These values are applied relative to it's parent, i.e.
 * as if the parent was the origin of coorinates. Because of that, if a parent
 * is moved/rotated, the children will also be affected, which allows to organize
 * the structure basing on semantical hierarchy.
*/
class Node : public std::enable_shared_from_this<Node>{
private:

	// A link to the parent. It's a weak_ptr, to break cyclic references.
	std::weak_ptr<Node> parent;
	// 3D position of this Node - relative to parent.
	glm::vec3 position;
	// Scale and rotation of this Node.
	float scale;
	glm::quat rotation;
	// Transform matrices for this node.
	glm::mat4 transform;
	void UpdateTransform();
	// If false, the subtree of this node will be skipped in processing.
	bool active = true;
	virtual void SetParent(std::shared_ptr<Node>);
protected:

public:
	Node() {};
	Node(glm::vec3 relative_pos);
	// The list of children nodes.
	std::list<std::shared_ptr<Node>> children;

	// Returns whether this node should be processed.
	bool GetActive() const;
	// Access for node transformation parameters
	glm::vec3 GetPosition() const;
	float GetScale() const;
	glm::quat GetRotation() const;
	void SetPosition(glm::vec3);
	void SetScale(float);
	void SetRotation(glm::quat);
	void SetActive(bool);
	// Sets position in absolute coordinates. Useful for physics calculations,
	// as they are performed in absolute coordinates exclusivelly.
	//void SetPosAbsolute(glm::vec2);
	// This function returns a relative transformation matrix.
	glm::mat4 GetTransform() const;
	// Creates a parent-children relationship. Adds a child node, and set's
	// its parent to this node.
	virtual void AddChild(std::shared_ptr<Node>);
	// Cleanup procedure. Removes patent-child relationship of this node and
	// it's parent. This may cause shared_ptrs to zero their use count,
	// which in turn would destruct all this subtree. Useful for removing
	// parts of a tree.
	void DetachFromParent();
	// A helper function that spawns a new Node managed by a shared_ptr.
	static std::shared_ptr<Node> Create(glm::vec3 pos = glm::vec3(0.0));
};

/* A node group is a helper node that stores multiple nodes
 * as if they were just one node. Elements that are made up of several
 * nodes are adviesed to derive from this class, it makes it easier
 * to maintains such tree if some nodes are grouped (like ball's body and drawable).
*/
class NodeGroup : public Node{
	// The pointer to the root of this group
	std::shared_ptr<Node> top;
	// Where child nodes for this group should be attached.
	std::shared_ptr<Node> bottom;
public:
	// Tell the group where it's own inner root is.
	void SetTop(std::shared_ptr<Node> top);
	// Tell the group where it's children should be attached.
	void SetBottom(std::shared_ptr<Node> bottom);
	// A custom version of AddChild, that attaches children to the node
	// designated by SetBottom.
	virtual void AddChild(std::shared_ptr<Node>);
};

#endif //POSITIONABLE_H
