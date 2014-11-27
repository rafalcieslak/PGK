#ifndef TEXT_HPP
#define TEXT_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Node.hpp"
#include "Signal.hpp"

/* This Node is similar to a Drawable, but instead of representing a VBO
 * model, it is used to render text.
*/

/* Temporarily disabled
class Text : public Node{
public:
	Text(std::string text, glm::vec2 px_offset, unsigned int size = 24, glm::vec3 color = glm::vec3(0.0,0.0,0.0), glm::vec2 relative_pos = glm::vec2(0.0,0.0));
	~Text();
	// Update the text this element displays.
	void SetText(std::string text);
	// The text buffer.
	std::string text;
	// Color for rendering.
	glm::vec3 color;
	// Font size.
	unsigned int size;
	// Offset expressed in pixel size, for perfect allignment.
	glm::vec2 px_offset;
	// Helper creator function.
	static std::shared_ptr<Text> Create(std::string text, glm::vec2 px_offset, unsigned int size = 24, glm::vec3 color = glm::vec3(0.0,0.0,0.0), glm::vec2 relative_pos = glm::vec2(0.0,0.0));
private:
	friend class Render;
	// The list of all texts that need to be rendered, for easier iteration.
	static std::vector<Text *> texts;
};
*/

#endif //TEXT_HPP
