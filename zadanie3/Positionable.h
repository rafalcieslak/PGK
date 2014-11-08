#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <list>

class Positionable : public std::enable_shared_from_this<Positionable>{
protected:
	std::list<std::shared_ptr<Positionable>> children;
	std::shared_ptr<Positionable> parent = nullptr;
	glm::vec2 relative_pos;
	glm::vec2 relative_scale = glm::vec2(1.0,1.0);
	glm::vec4 GetPosScale();
public:
	Positionable() {};
	Positionable(glm::vec2 relative_pos);
	glm::vec2 GetPos();
	glm::vec2 GetScale();
	glm::vec2& GetPosRelative();
	glm::vec2& GetScaleRelative();
	void SetPosRelative(glm::vec2);
	void SetPosAbsolute(glm::vec2);
	void SetScale(glm::vec2);
	virtual void LinkChild(std::shared_ptr<Positionable>);
	virtual void SetParent(std::shared_ptr<Positionable>);
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
