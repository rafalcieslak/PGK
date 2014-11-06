#include "ModelBase.h"

Model::Model(unsigned int n, float* v, float* c): size(n){
	vertices = new float[n*2];
	colors   = new float[n*3];
	for(unsigned int i = 0; i < n*2; i++) vertices[i] = v[i];
	for(unsigned int i = 0; i < n*3; i++)   colors[i] = c[i];
}

Model::~Model(){
	delete vertices;
	delete colors;
}


void ModelBase::AddModel(std::string id, unsigned int vertices, float* coords, float* colors){
	std::shared_ptr<Model> m = std::make_shared<Model>(vertices,coords,colors);
	models[id] = m;
}

void ModelBase::Clear(){
	models.clear();
}

bool ModelBase::HasModel(std::string id){
	if(models.find(id) != models.end()) return true;
	return false;
}
