#ifndef OBJPARSER_HPP
#define OBJPARSER_HPP

#include <fstream>
#include <map>
#include "Mesh.hpp"

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
