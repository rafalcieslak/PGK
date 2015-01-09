#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "engine/Drawable.hpp"

class Background : public PositionableGroup{
private:
	class BackgroundDrawableTile : public Drawable{
	public:
		BackgroundDrawableTile();
	};
	class BackgroundDrawableLines : public Drawable{
	public:
		BackgroundDrawableLines();
	};
	std::vector<std::shared_ptr<BackgroundDrawableTile>> drawables;
	std::vector<std::shared_ptr<BackgroundDrawableLines>> drawables_l;
	Background();
	void init(float a, int w, int h);
	std::shared_ptr<Positionable> root;
public:
	static std::shared_ptr<Background> Create(float scale, int width, int height);
};

#endif //BACKGROUND_HPP
