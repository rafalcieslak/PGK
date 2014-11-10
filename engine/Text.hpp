#ifndef TEXT_HPP
#define TEXT_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Positionable.hpp"
#include "Signal.hpp"

class Text : public Positionable{
public:
	Text(std::string text, glm::vec2 px_offset, unsigned int size = 24, glm::vec3 color = glm::vec3(0.0,0.0,0.0), glm::vec2 relative_pos = glm::vec2(0.0,0.0));
	~Text();
	void SetText(std::string text);
	std::string text;
	glm::vec3 color;
	unsigned int size;
	glm::vec2 px_offset;
	static std::shared_ptr<Text> Create(std::string text, glm::vec2 px_offset, unsigned int size = 24, glm::vec3 color = glm::vec3(0.0,0.0,0.0), glm::vec2 relative_pos = glm::vec2(0.0,0.0));
private:
	friend class Render;
	static std::vector<Text *> texts;
};

#endif //TEXT_HPP
