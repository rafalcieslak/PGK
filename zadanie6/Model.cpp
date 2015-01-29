#include "Model.hpp"
#include "ObjLoader.hpp"
#include <iostream>

std::shared_ptr<Model> Model::Create(std::string objpath){


	std::shared_ptr<Model> m(new Model());
	if(!LoadObj(objpath.c_str(),m->vertices,m->uvs,m->normals)){
		return nullptr;
	}
	return m;
}

void Model::Prepare(){
	std::cout << vertices.size() <<" " << uvs.size() << " " << normals.size() << std::endl;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

}

void Model::Render(){

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


}

Model::Model(){


}
