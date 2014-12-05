#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <string>
#include "Node.hpp"
#include "Signal.hpp"

/* A drawable represents a visible object in a tree of Nodes.
 * The model_id field points to a model in ModelBase which has to be drawn in order
 * to display this Drawable. It the model has multiple color variants, the
 * variant field can be used to choose one.
*/
class Drawable : public Node{
public:
	Drawable(std::string id = "", glm::vec3 relative_pos = glm::vec3(0.0), unsigned int variant = 0);
	~Drawable();
	// Begins an animation for this drawable
	void StartAnimation(unsigned int mode, double length);
	// The name of model in ModelBase this drawable is an instance of.
	std::string model_id;
	// The color variant of the model.
	unsigned int variant;
	// This signal is emitted when an animation for this drawable has been completed.
	Signal<int> on_animation_finished;
	// 0 - no culling, 1 - cull backfaces, 2 - cull frontfaces
	unsigned int culling = 0;
	// Lighting properties
	float spatial = 1.1, diffuse = 1.1;
private:
	friend class Render;
	// Animation state.
	unsigned int anim_mode = 0;
	double anim_start;
	double anim_length;
};

#endif //DRAWABLE_H
