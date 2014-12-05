#ifndef BUBBLE_HPP
#define BUBBLE_HPP

#include "../engine/Drawable.hpp"

class Bubble : public Drawable{
public:
	Bubble(float scale = 1.0);
	void RotateTowards(glm::vec3);
	void ApplyMovement(float time_delta);
	void ApplyScale();
	bool ShouldPop() const;
	float DistanceToCamera() const;
private:
	static float ZPosToH(float zpos);
	static float HToZPos(float h);
	static float SpeedFunc(float h);
	static float ScaleFunc(float h);
};

#endif //BUBBLE_HPP
