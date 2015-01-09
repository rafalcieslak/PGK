#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Positionable.hpp"
#include "Signal.hpp"

/* A drawable represents a visible object in a tree of Positionables.
 * The model_id field points to a model in ModelBase which has to be drawn in order
 * to display this Drawable. It the model has multiple color variants, the
 * variant field can be used to choose one.
*/
class Drawable : public Positionable{
public:
	Drawable(std::string id, glm::vec2 relative_pos = glm::vec2(0.0,0.0), unsigned int variant = 0);
	~Drawable();
	// Begins an animation for this drawable
	void StartAnimation(unsigned int mode, double length);
	// The name of model in ModelBase this drawable is an instance of.
	std::string model_id;
	// The color variant of the model.
	unsigned int variant;
	// This signal is emitted when an animation for this drawable has been completed.
	Signal<int> on_animation_finished;
private:
	friend class Render;
	// A list of pointers to all drawables created. Useful for quickly iterating over
	// all things that need to be drawn.
	static std::vector<Drawable *> drawables;
	// Animation state.
	unsigned int anim_mode = 0;
	double anim_start;
	double anim_length;
};

#endif //DRAWABLE_H
