#ifndef MODEL_BASE_H
#define MODEL_BASE_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define MODEL_DIMEN 3
#define MODEL_COLOR_SIZE 4

/* A model represents a set of vertices and colors that is present in a VBO
 * and thus can be easily drawn. Models are usually created and used by Drawable
 * specialisations. They are stored in a map string->Model that is managed by
 * ModelBase, so that multiple drawables that use the same model can relay on
 * the same intance of data in VBO.
*/
class Model{
public:
	// Several constructors.
	// ppp - points per primitive. 3 for GL_TRIANGLES, 2 for GL_LINES.
	// mode - GL mode to use for drawing this model.
	// vertices - the number of vertices in this particular model.
	// v - list of vertices, as std::initializer_list or a const array.
	// c - list of colors for vertices, in the same format as v. It can be a
	//     two-dimentional initializer_list, so that the model could faciliate
	//     multiple color variants.
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> n, std::initializer_list<std::initializer_list<float>> c);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, std::initializer_list<float> v, std::initializer_list<std::initializer_list<float>> c);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> n, std::initializer_list<float> c);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> c);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, const float* v, const float* n, const float* c);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, const float* v, const float* n, glm::vec4 color);
	Model(unsigned int ppp, GLenum mode, unsigned int vertices, const float* v, const float* n, std::initializer_list<glm::vec4> color);
	// Never implicitly copy a model, because it needs a VBO, and it allocates
	// memory, too.
	Model(const Model& other) = delete;
	void operator=(const Model& other) = delete;
	~Model();

	friend class Render;
private:
	// Data copies in memory.
	float *vertices = nullptr, *normals = nullptr, **colors = nullptr;
	// Number of vertices and color variants.
	unsigned int size, variants;
	bool single_color = false;
	// VBO indices.
	GLuint buffer_vertex, *buffers_color = nullptr, buffer_normals;
	glm::vec4* boring_colors = nullptr;
	// Helper functions for initialization.
	void init_buffers();
	void init_arrays();
	// Points per primitive.
	unsigned int ppp;
	// Drawing mode (such as GL_LINES or GL_TRIANGLES).
	GLenum mode;
};

// Specialized Model for models that are made up of triangles.
class ModelTriangles : public Model{
public:
	ModelTriangles(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> n, std::initializer_list<std::initializer_list<float>> c) :
		Model(3, GL_TRIANGLES, vertices, v, n, c) {};
	ModelTriangles(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<std::initializer_list<float>> c) :
		Model(3, GL_TRIANGLES, vertices, v, c) {};
	ModelTriangles(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> n, std::initializer_list<float> c):
		Model(3, GL_TRIANGLES, vertices, v, n, c) {};
	ModelTriangles(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> c):
		Model(3, GL_TRIANGLES, vertices, v, c) {};
	ModelTriangles(unsigned int vertices, const float* v, const float* n, const float* c):
		Model(3, GL_TRIANGLES, vertices, v, n, c) {};
	ModelTriangles(unsigned int vertices, const float* v, const float* n, glm::vec4 color):
		Model(3, GL_TRIANGLES, vertices, v, n, color) {};
	ModelTriangles(unsigned int vertices, const float* v, const float* n, std::initializer_list<glm::vec4> color):
		Model(3, GL_TRIANGLES, vertices, v, n, color) {};

};

// Specialized Model for models that are made up of lines.
class ModelLines : public Model{
public:
	ModelLines(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> n, std::initializer_list<std::initializer_list<float>> c) :
		Model(2, GL_LINES, vertices, v, n, c) {};
	ModelLines(unsigned int vertices, std::initializer_list<float> v, std::initializer_list<float> n, std::initializer_list<float> c):
		Model(2, GL_LINES, vertices, v, n, c) {};
	ModelLines(unsigned int vertices, const float* v, const float* n, const float* c):
		Model(2, GL_LINES, vertices, v, n, c) {};
};

/* The singleton catalogue of all models, organised by their unique names.
*/
class ModelBase{
private:
	// This is a singleton class.
	ModelBase(){};
	ModelBase(const ModelBase&) = delete;
	void operator=(const ModelBase&) = delete;
	std::map<std::string, std::shared_ptr<Model>> models;
public:
	// Returns the singleton instance.
	static ModelBase& GetInstance(){
		static ModelBase instance;
		return instance;
	}
	// Create and append a new model. See Model constructors.
	void AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> normals, std::initializer_list<std::initializer_list<float>> colors);
	void AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<std::initializer_list<float>> colors);
	void AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> normals, std::initializer_list<float> colors);
	void AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> colors);
	void AddModelTriangles(std::string id, unsigned int vertices, const float *coords, const float* normals, const float* colors);
	void AddModelTriangles(std::string id, unsigned int vertices, const float *coords, const float* normals, glm::vec4 color);
	void AddModelTriangles(std::string id, unsigned int vertices, const float *coords, const float* normals, std::initializer_list<glm::vec4> color);
	void AddModelLines(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> normals, std::initializer_list<std::initializer_list<float>> colors);
	void AddModelLines(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> normals, std::initializer_list<float> colors);
	void AddModelLines(std::string id, unsigned int vertices, const float *coords, const float* normals, const float* colors);
	// Remove all models, cleanup.
	void Clear();
	// Search for a model.
	std::shared_ptr<Model> GetModel(std::string id) const;
	// Check if a model exists in base.
	bool HasModel(std::string id) const;
};

#endif //MODEL_BASE_H
