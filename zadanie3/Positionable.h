#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#include <glm/glm.hpp>
#include <list>

class Positionable : public std::enable_shared_from_this<Positionable>{
private:
	std::list<std::shared_ptr<Positionable>> children;
	std::shared_ptr<Positionable> parent = nullptr;
	glm::vec2 relative_pos;
public:
	Positionable();
	Positionable(std::shared_ptr<Positionable> parent, glm::vec2 relative_pos);
	glm::vec2 GetPos();
	glm::vec2 GetPosRelative();
	glm::vec2& PosRelative();
	void SetPosRelative(glm::vec2);
	void SetPosAbsolute(glm::vec2);
	void AddChild(std::shared_ptr<Positionable>);
};

#endif //POSITIONABLE_H
