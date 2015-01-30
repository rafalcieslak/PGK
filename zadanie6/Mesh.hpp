#ifndef MESH_HPP
#define MESH_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <string>
#include <memory>

typedef glm::vec3 vert;
typedef glm::vec2 vert_tex;
typedef glm::vec3 vert_normal;
typedef struct{
	vert v;
	vert_tex vt;
	vert_normal vn;
	bool no_normal = false;
} face_vertex;
typedef std::array<face_vertex,3> trig;
typedef glm::vec3 mat_color;
typedef struct{
	glm::vec3 min = glm::vec3(1e30,1e30,1e30);
	glm::vec3 max = glm::vec3(-1e30,-1e30,-1e30);
	void Update(glm::vec3 t){
		if(t.x > max.x) max.x = t.x;
		if(t.x < min.x) min.x = t.x;
		if(t.y > max.y) max.y = t.y;
		if(t.y < min.y) min.y = t.y;
		if(t.z > max.z) max.z = t.z;
		if(t.z < min.z) min.z = t.z;
	}
	glm::vec3 Center() const{ return 0.5f*(max+min); }
	float Radius() const{
		glm::vec3 q = Center() - max;
		return glm::pow(q.x*q.x + q.z*q.z + q.y*q.y, 0.5f);
	}
} bounds;

struct Material{
	mat_color ambient;
	mat_color diffuse;
	mat_color spectral;
	std::string  ambient_tex_path;
	std::string  diffuse_tex_path;
	std::string spectral_tex_path;
	float spectral_exponent = 25.0;
	std::string name;
	float alpha = 1.0f;
};

class Mesh{
public:
	std::vector<trig> faces;
	std::shared_ptr<Material> material;
	void Render() const;
	void PrepareData();
	void PrepareBuffers();
	std::string name;
	bool hidden = false;
	bounds GetBounds() const;
	void Translate(glm::vec3);
private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	unsigned int vertexbuffer, uvbuffer, normalbuffer;
};

#endif //MESH_HPP
