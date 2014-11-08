#include "ModelBase.h"
#include "Render.h"
#include <iostream>

Model::Model(unsigned int n, const float* v, const float* c): size(n){
	if(!Render::inited) Render::Init(); // To avoid problems when object constructor are called before init()
	vertices = new float[n*6];
	colors   = new float[n*9];
	for(unsigned int i = 0; i < n*6; i++) vertices[i] = v[i];
	for(unsigned int i = 0; i < n*9; i++)   colors[i] = c[i];
	glGenBuffers(1, &buffer_vertex);
	glGenBuffers(1, &buffer_color);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*n*6, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*n*9, colors, GL_STATIC_DRAW);
}

Model::~Model(){
	glDeleteBuffers(1, &buffer_vertex);
	glDeleteBuffers(1, &buffer_color);
	delete vertices;
	delete colors;
}


void Model::metaDraw(){

	// Select buffers to use
	glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	glBindBuffer(GL_ARRAY_BUFFER, buffer_color);
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	// Draw the model
	glDrawArrays(GL_TRIANGLES, 0, 3*size);
}

void ModelBase::AddModel(std::string id, unsigned int vertices, const float* coords, const float* colors){
	std::shared_ptr<Model> m = std::make_shared<Model>(vertices,coords,colors);
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
