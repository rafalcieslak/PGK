#include "ModelBase.hpp"
#include "Render.hpp"

Model::Model(unsigned int p, GLenum m, unsigned int n, std::initializer_list<float> v, std::initializer_list<std::initializer_list<float>> c): size(n), ppp(p), mode(m){
	variants = c.size();
	init_arrays();
	auto vi = v.begin();
	auto ci_ = c.begin();
	for(unsigned int i = 0; i < size*ppp*MODEL_DIMEN; i++) vertices[i] = *(vi++);
	for(unsigned int w = 0; w < variants; w++){
		auto ci = (*(ci_++)).begin();
		for(unsigned int i = 0; i < size*ppp*MODEL_COLOR_SIZE; i++)  colors[w][i] = *(ci++);
	}
	init_buffers();
}

Model::Model(unsigned int p, GLenum m, unsigned int n, std::initializer_list<float> v, std::initializer_list<float> c): size(n), ppp(p), mode(m){
	variants = 1;
	init_arrays();
	auto vi = v.begin();
	auto ci = c.begin();
	for(unsigned int i = 0; i < size*ppp*MODEL_DIMEN; i++) vertices[i] = *(vi++);
	for(unsigned int i = 0; i < size*ppp*MODEL_COLOR_SIZE; i++) colors[0][i] = *(ci++);
	init_buffers();
}

Model::Model(unsigned int p, GLenum m, unsigned int n, const float* v, const float* c): size(n), ppp(p), mode(m){
	variants = 1;
	init_arrays();
	for(unsigned int i = 0; i < size*ppp*MODEL_DIMEN; i++) vertices[i] = v[i];
	for(unsigned int i = 0; i < size*ppp*MODEL_COLOR_SIZE; i++) colors[0][i] = c[i];
	init_buffers();
}

void Model::init_arrays(){
	if(!Render::inited) Render::Init(); // To avoid problems when object constructor are called before init()
	vertices = new float[size*ppp*MODEL_DIMEN];
	colors = new float*[variants];
	for(unsigned int w = 0; w < variants; w++)
		colors[w]   = new float[size*ppp*MODEL_COLOR_SIZE];
}
void Model::init_buffers(){
	buffers_color = new GLuint[variants];
	glGenBuffers(1, &buffer_vertex);
	glGenBuffers(variants, buffers_color);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size*ppp*MODEL_DIMEN, vertices, GL_STATIC_DRAW);
	for(unsigned int i = 0; i < variants; i++){
		glBindBuffer(GL_ARRAY_BUFFER, buffers_color[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size*ppp*MODEL_COLOR_SIZE, colors[i], GL_STATIC_DRAW);
	}
}


Model::~Model(){
	glDeleteBuffers(1, &buffer_vertex);
	glDeleteBuffers(variants, buffers_color);
	delete[] buffers_color;
	for(unsigned int w = 0; w < variants; w++){
		delete[] colors[w];
	}
	delete[] vertices;
	delete[] colors;
}


void Model::metaDraw(unsigned int variant){
	variant %= variants;
	// Select buffers to use
	glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
	glVertexAttribPointer( 0, MODEL_DIMEN, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	glBindBuffer(GL_ARRAY_BUFFER, buffers_color[variant]);
	glVertexAttribPointer( 1, MODEL_COLOR_SIZE, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	// Draw the model
	glDrawArrays(mode, 0, ppp*size);
}

void ModelBase::AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> colors){
	std::shared_ptr<ModelTriangles> m = std::make_shared<ModelTriangles>(vertices,coords,colors);
	models[id] = m;
}
void ModelBase::AddModelTriangles(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<std::initializer_list<float>> colors){
	std::shared_ptr<ModelTriangles> m = std::make_shared<ModelTriangles>(vertices,coords,colors);
	models[id] = m;
}
void ModelBase::AddModelTriangles(std::string id, unsigned int vertices, const float* coords, const float* colors){
	std::shared_ptr<ModelTriangles> m = std::make_shared<ModelTriangles>(vertices,coords,colors);
	models[id] = m;
}
void ModelBase::AddModelLines(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<float> colors){
	std::shared_ptr<ModelLines> m = std::make_shared<ModelLines>(vertices,coords,colors);
	models[id] = m;
}
void ModelBase::AddModelLines(std::string id, unsigned int vertices, std::initializer_list<float> coords, std::initializer_list<std::initializer_list<float>> colors){
	std::shared_ptr<ModelLines> m = std::make_shared<ModelLines>(vertices,coords,colors);
	models[id] = m;
}
void ModelBase::AddModelLines(std::string id, unsigned int vertices, const float* coords, const float* colors){
	std::shared_ptr<ModelLines> m = std::make_shared<ModelLines>(vertices,coords,colors);
	models[id] = m;
}

void ModelBase::Clear(){
	models.clear();
}

bool ModelBase::HasModel(std::string id) const{
	if(models.find(id) != models.end()) return true;
	return false;
}


std::shared_ptr<Model> ModelBase::GetModel(std::string id) const{
	auto it = models.find(id);
	if(it != models.end()) return it->second;
	return nullptr;
}
