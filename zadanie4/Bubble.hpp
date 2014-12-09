#ifndef BUBBLE_HPP
#define BUBBLE_HPP

#include "../engine/Drawable.hpp"

class Bubble : public NodeGroup{
public:
	void RotateTowards(glm::vec3);
	void ApplyMovement(float time_delta);
	void ApplyScale();
	bool ShouldPop() const;
	void AddLight();
	float DistanceToCamera() const;
	bool IsPointInside(glm::vec3) const;
	bool HasLight() const {return has_light;}
	float sizemult = 1.0;
	static std::shared_ptr<Bubble> Create(float scale);
	std::shared_ptr<Drawable> GetDrawable() const {return drawable;}
private:
	class BubbleDrawable : public Drawable{
	public:
		BubbleDrawable();
	};
	std::shared_ptr<BubbleDrawable> drawable;
	static float ZPosToH(float zpos);
	static float HToZPos(float h);
	static float SpeedFunc(float h);
	static float ScaleFunc(float h);
	Bubble(float scale = 1.0);
	void init();
	bool has_light = false;
};

#endif //BUBBLE_HPP
