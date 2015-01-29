#ifndef OBJPARSER_HPP
#define OBJPARSER_HPP

#include <fstream>
#include <map>
#include "Mesh.hpp"

class MaterialLibrary{
public:
	MaterialLibrary(std::string mtlpath);
	~MaterialLibrary();
	// Parses the whole file
	void Parse();
	// Parses a single line. Returns true if there is more data available.
	bool Step();
	// Tests whether the file exists and is readable
	bool Good();
	// Returns a pointer to a material by its name
	std::shared_ptr<Material> GetMaterial(std::string);
private:
	// The file stream
	std::ifstream file;
	// Begins a new material
	void StartNewMaterial(std::string name);
	// The base of materials
	std::map<std::string, std::shared_ptr<Material>> material_map;
	// The currently parsed material
	std::shared_ptr<Material> current_material;

	// Location of the MTL file
	std::string path;
	std::string dir;
};


class ObjParser{
public:
	ObjParser(std::string objpath);
	~ObjParser();

	// Parses the whole file
	void Parse();
	// Parses a single line.  Returns true if there is more data available.
	bool Step();
	// Tests whether the file exists and is readable
	bool Good();

	// Parsed meshes land here
	std::vector<std::shared_ptr<Mesh>> meshes;
private:
	// The file stream
	std::ifstream file;
	// The currently parsed mesh
	std::shared_ptr<Mesh> current_mesh;
	// Tells if the current mesh is empty or not
	bool mesh_nonempty = false;
	// The currently used material library
	std::shared_ptr<MaterialLibrary> current_mtllib;
	// Begins a new mesh
	void StartNewMeshIfNotEmpty();

	// Location of the OBJ file
	std::string path;
	std::string dir;

	// The last parsed line number
	int lineno = 0;

	face_vertex ParseFW(std::string);
	trig FixMissingNormals(trig);

	std::vector<vert> vertices;
	std::vector<vert_tex> textures;
	std::vector<vert_normal> normals;
};

#endif //OBJPARSER
