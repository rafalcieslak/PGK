#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

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
	glm::vec3 scale = glm::vec3(1.0,1.0,1.0);
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
	Node(glm::vec3 position);
	Node(float x, float y, float z) : Node(glm::vec3(x,y,z)) {}
	// The list of children nodes.
	std::vector<std::shared_ptr<Node>> children;

	// Returns whether this node should be processed.
	bool GetActive() const;
	// Access for node transformation parameters
	glm::vec3 GetPosition() const;
	glm::vec3 GetScale() const;
	glm::quat GetRotation() const;
	void SetPosition(glm::vec3);
	void SetPosition(float x, float y, float z) {SetPosition(glm::vec3(x,y,z));}
	void SetScale(glm::vec3);
	void SetScale(float x, float y, float z) {SetScale(glm::vec3(x,y,z));}
	void SetScale(float s) {SetScale(glm::vec3(s,s,s));}
	void SetRotation(glm::quat);
	void SetActive(bool);
	// Sets position in absolute coordinates. Useful for physics calculations,
	// as they are performed in absolute coordinates exclusivelly.
	//void SetPosAbsolute(glm::vec2);
	// This function returns a relative transformation matrix.
	glm::mat4 GetTransform() const;
	glm::mat4 GetGlobalTransform() const;
	glm::vec3 GetGlobalPos() const {
		glm::vec4 v = GetGlobalTransform() * glm::vec4(0.0,0.0,0.0,1.0);
		return glm::vec3(v.x,v.y,v.z);
	}
	// Creates a parent-children relationship. Adds a child node, and set's
	// its parent to this node.
	virtual void AddChild(std::shared_ptr<Node>);
	virtual void RemoveChild(std::shared_ptr<Node>);
	// Orders the children using the function given as parameter.
	void SortChildren(std::function<float(std::shared_ptr<Node>)> value_func);
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
	virtual void RemoveChild(std::shared_ptr<Node>);
};

#endif //POSITIONABLE_H
