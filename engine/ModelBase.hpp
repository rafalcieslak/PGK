#ifndef MODEL_BASE_H
#define MODEL_BASE_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>

class Model{
public:
	Model(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<std::initializer_list<float>> c);
	Model(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> c);
	Model(unsigned int vertices, const float* v, const float* c);
	Model(const Model& other) = delete;
	void operator=(const Model& other) = delete;
	~Model();
	void metaDraw(unsigned int variant = 1);
private:
	float *vertices = nullptr, **colors = nullptr;
	unsigned int size, variants;
	GLuint buffer_vertex, *buffers_color = nullptr;
	void init_buffers();
	void init_arrays();
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
	void AddModel(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<std::initializer_list<float>> colors);
	void AddModel(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> colors);
	void AddModel(std::string id, unsigned int vertices, const float *coords, const float* colors);
	bool HasModel(std::string id) const;
	void Clear();
	std::shared_ptr<Model> GetModel(std::string id) const;
};

#endif //MODEL_BASE_H
