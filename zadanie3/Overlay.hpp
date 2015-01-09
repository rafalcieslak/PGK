#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include "engine/Drawable.hpp"

class Overlay : public Drawable{
private:
	Overlay();
public:
	static std::shared_ptr<Overlay> Create();
};

#endif //OVERLAY_HPP
