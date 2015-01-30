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

struct Material{
	mat_color ambient;
	mat_color diffuse;
	mat_color spectral;
	std::string  ambient_tex_path;
	std::string  diffuse_tex_path;
	std::string spectral_tex_path;
	float spectral_exponent = 25.0;
	std::string name;
};

class Mesh{
public:
	std::vector<trig> faces;
	std::shared_ptr<Material> material;
	void Render();
	void PrepareBuffers();
	std::string name;
	bool hidden = false;
	float GetMaximumDistanceFromOriginSquared();
private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	unsigned int vertexbuffer, uvbuffer, normalbuffer;
};

#endif //MESH_HPP
