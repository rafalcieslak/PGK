#ifndef OBJPARSER_HPP
#define OBJPARSER_HPP

#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <map>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

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
	float spectral_exponent;
	std::string name;
};

class MaterialLibrary{
public:
	MaterialLibrary(std::string mtlpath);
	void Parse();
	bool Step();
	bool Good();
	std::shared_ptr<Material> GetMaterial(std::string);
private:
	void StartNewMaterial(std::string name);
	std::map<std::string, std::shared_ptr<Material>> material_map;
	std::ifstream file;
	std::shared_ptr<Material> current_material;
};

class Mesh{
public:
	std::vector<trig> faces;
	std::shared_ptr<Material> material;
	void Render();
	void PrepareBuffers();
	std::string name;
private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
};

class ObjParser{
public:
	ObjParser(std::string objpath);
	~ObjParser();

	bool Good();
	bool Step(); // Returns false on EOF or critical error
	void Parse();

	std::vector<std::shared_ptr<Mesh>> meshes;
private:
	std::ifstream file;
	std::shared_ptr<Mesh> current_mesh;
	bool mesh_nonempty = false;
	std::shared_ptr<MaterialLibrary> current_mtllib;
	void StartNewMeshIfNotEmpty();
	std::string path;
	int lineno = 0;

	face_vertex ParseFW(std::string);
	trig FixMissingNormals(trig);

	std::vector<vert> vertices;
	std::vector<vert_tex> textures;
	std::vector<vert_normal> normals;
};

#endif //OBJPARSER
