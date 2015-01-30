#include "Mesh.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void Mesh::PrepareBuffers(){
	for(const trig &t : faces){
		positions.push_back(t[0].v);
		positions.push_back(t[1].v);
		positions.push_back(t[2].v);
		uvs.push_back(t[0].vt);
		uvs.push_back(t[1].vt);
		uvs.push_back(t[2].vt);
		normals.push_back(t[0].vn);
		normals.push_back(t[1].vn);
		normals.push_back(t[2].vn);
	}

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

void Mesh::Render(){

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);

	glDrawArrays(GL_TRIANGLES, 0, positions.size());


}

float Mesh::GetMaximumDistanceFromOriginSquared(){
	float max = 0.0;
	for(auto t: positions){
		float d = t.x*t.x + t.y*t.y + t.z*t.z;
		if(d > max) max = d;
	}
	return max;
}
