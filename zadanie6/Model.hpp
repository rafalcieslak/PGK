#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Model{
public:
	static std::shared_ptr<Model> Create(std::string objpath, std::string texpath);
	void Prepare();
	void Render();
private:
	Model();
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer, uvbuffer, normalbuffer;
	GLuint texture;
};

#endif //MODEL_HPP
