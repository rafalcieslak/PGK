#ifndef MODEL_BASE_H
#define MODEL_BASE_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>

#define MODEL_DIMEN 2
#define MODEL_COLOR_SIZE 3

class Model{
public:
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, std::initializer_list<float> v, std::initializer_list<std::initializer_list<float>> c);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> c);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, const float* v, const float* c);
	Model(const Model& other) = delete;
	void operator=(const Model& other) = delete;
	~Model();
	void metaDraw(unsigned int variant = 0);
private:
	float *vertices = nullptr, **colors = nullptr;
	unsigned int size, variants;
	GLuint buffer_vertex, *buffers_color = nullptr;
	void init_buffers();
	void init_arrays();
	unsigned int ppp; // points per primitive
	GLenum mode; // drawing mode (such as GL_LINES or GL_TRIANGLES)
};

class ModelTriangles : public Model{
public:
	ModelTriangles(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<std::initializer_list<float>> c) :
		Model(3, GL_TRIANGLES, vertices, v, c) {};
	ModelTriangles(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> c):
		Model(3, GL_TRIANGLES, vertices, v, c) {};
	ModelTriangles(unsigned int vertices, const float* v, const float* c):
		Model(3, GL_TRIANGLES, vertices, v, c) {};

};

class ModelLines : public Model{
public:
	ModelLines(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<std::initializer_list<float>> c) :
		Model(2, GL_LINES, vertices, v, c) {};
	ModelLines(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> c):
		Model(2, GL_LINES, vertices, v, c) {};
	ModelLines(unsigned int vertices, const float* v, const float* c):
		Model(2, GL_LINES, vertices, v, c) {};
};

class ModelBase{
private:
	ModelBase(){};
	ModelBase(const ModelBase&) = delete;
	void operator=(const ModelBase&) = delete;
	std::map<std::string, std::shared_ptr<Model>> models;
public:
	static ModelBase& GetInstance(){
		static ModelBase instance;
		return instance;
	}
	void AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<std::initializer_list<float>> colors);
	void AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> colors);
	void AddModelTriangles(std::string id, unsigned int vertices, const float *coords, const float* colors);
	void AddModelLines(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<std::initializer_list<float>> colors);
	void AddModelLines(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> colors);
	void AddModelLines(std::string id, unsigned int vertices, const float *coords, const float* colors);
	bool HasModel(std::string id) const;
	void Clear();
	std::shared_ptr<Model> GetModel(std::string id) const;
};

#endif //MODEL_BASE_H
