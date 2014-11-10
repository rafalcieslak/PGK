#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <list>
#include <memory>
#include <algorithm>

glm::vec2 Rotate2dVector01(glm::vec2, float);

class Positionable : public std::enable_shared_from_this<Positionable>{
protected:
	std::shared_ptr<Positionable> parent = nullptr;
	glm::vec2 relative_pos;
	float relative_scale = 1.0;
	float relative_angle = 0.0;
	glm::vec4 GetPosScaleAngle() const;
	bool active = true;
public:
	Positionable() {};
	Positionable(glm::vec2 relative_pos);
	std::list<std::shared_ptr<Positionable>> children;

	glm::vec2 GetPos() const;
	float GetScale() const;
	float GetAngle() const;
	glm::vec2& GetPosRelative();
	float& GetScaleRelative();
	bool& GetActiveRelative();
	bool GetActiveAbsolute();
	void SetPosRelative(glm::vec2);
	void SetPosAbsolute(glm::vec2);
	void SetScale(float);
	void SetAngle(float);
	void SetActive(bool);
	virtual void LinkChild(std::shared_ptr<Positionable>);
	virtual void SetParent(std::shared_ptr<Positionable>);
	static std::shared_ptr<Positionable> Create(glm::vec2 pos = glm::vec2(0.0,0.0));
};

class PositionableGroup : public Positionable{
	std::shared_ptr<Positionable> top;
	std::shared_ptr<Positionable> bottom;
public:
	void SetTop(std::shared_ptr<Positionable> top);
	void SetBottom(std::shared_ptr<Positionable> bottom);
	virtual void LinkChild(std::shared_ptr<Positionable>);
};

#endif //POSITIONABLE_H
