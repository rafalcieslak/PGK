#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Positionable.hpp"
#include "Signal.hpp"

class Drawable : public Positionable{
public:
	Drawable(std::string id, glm::vec2 relative_pos = glm::vec2(0.0,0.0), unsigned int variant = 0);
	~Drawable();
	void StartAnimation(unsigned int mode, double length);
	std::string model_id;
	unsigned int variant;
	Signal<int> on_animation_finished;
private:
	friend class Render;
	static std::vector<Drawable *> drawables;
	unsigned int anim_mode = 0;
	double anim_start;
	double anim_length;
};

#endif //DRAWABLE_H
