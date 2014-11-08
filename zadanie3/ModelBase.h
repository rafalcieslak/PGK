#ifndef MODEL_BASE_H
#define MODEL_BASE_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>

class Model{
public:
	Model(unsigned int vertices, float* v, float* c);
	~Model();
	float *vertices, *colors;
	unsigned int size;
	GLuint buffer_vertex, buffer_color;
	void metaDraw();
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
	void AddModel(std::string id, unsigned int vertices, float* coords, float* colors);
	bool HasModel(std::string id) const;
	void Clear();
	std::shared_ptr<Model> GetModel(std::string id) const;
};

#endif //MODEL_BASE_H
