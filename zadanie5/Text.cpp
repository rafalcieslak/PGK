#include "Text.hpp"
#include <algorithm>

std::vector<Text*> Text::texts;

Text::Text(std::string t, glm::vec2 px_off, unsigned int s, glm::vec3 c):
		text(t), color(c), size(s), px_offset(px_off){
	texts.emplace_back(this);
}

Text::~Text(){
	auto p = std::find(texts.begin(), texts.end(), this);
	if (p != texts.end())
		texts.erase(p);
}

void Text::SetText(std::string t){
	text = t;
}

std::shared_ptr<Text> Text::Create(std::string t, glm::vec2 px_off, unsigned int s, glm::vec3 c){
	return std::make_shared<Text>(t, px_off, s, c);
}
