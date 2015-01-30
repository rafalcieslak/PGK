#include "ObjParser.hpp"
#include <iostream>
#include <algorithm>

std::vector<std::string> SplitString(std::string str, std::string delimiter, bool skipempty = true);
std::string GetDir(const std::string& str);

ObjParser::ObjParser(std::string objname) : file(objname){
	path = objname;
	dir = GetDir(path);
	current_mesh = std::make_shared<Mesh>();
}

bool ObjParser::Good(){
	return (bool)file;
}

void ObjParser::Parse(){
	while(Step());
	StartNewMeshIfNotEmpty(); // ends the current one
}

void ObjParser::StartNewMeshIfNotEmpty(){
	if(mesh_nonempty){
		//std::cout << "Starting a new mesh." << std::endl;
		meshes.push_back(current_mesh);
		std::shared_ptr<Material> m = current_mesh->material;
		current_mesh = std::make_shared<Mesh>();
		current_mesh->material = m;
		mesh_nonempty = false;
	}
}


bool ObjParser::Step(){
	std::string line;
	if(!std::getline(file, line)) return false;
	lineno++;
	line.erase( std::remove_if(line.begin(), line.end(), [](char c){return c=='\r'||c=='\t';}), line.end() );
	auto split = SplitString(line, " ");
	if(split.size() < 1) return true;
	if(split[0][0] == '#' || split[0][0] == '\n' || split[0][0] == '\r'){
		// a comment or an empty line.
		return true;
	}else if(split[0] == "v"){
		// vertex entry
		vertices.push_back(vert(std::stof(split[1]),std::stof(split[2]),std::stof(split[3])));
	}else if(split[0] == "vn"){
		// vertex normal entry
		normals.push_back(vert_normal(std::stof(split[1]),std::stof(split[2]),std::stof(split[3])));
	}else if(split[0] == "vt"){
		// vertex texture
		textures.push_back(vert_tex(std::stof(split[1]),std::stof(split[2])));
	}else if(split[0] == "s"){
		//std::cout << "ObjParser " << lineno << ": ignoring smoothing group." << std::endl;
	}else if(split[0] == "o"){
		StartNewMeshIfNotEmpty();
		current_mesh->name = split[1];
	}else if(split[0] == "g"){
		// new group.
		// StartNewMeshIfNotEmpty();
	}else if(split[0] == "mtllib"){
		std::replace( split[1].begin(), split[1].end(), '\\', '/');
		std::string mtlfile = GetDir(path) + "/" + split[1];
		current_mtllib = std::make_shared<MaterialLibrary>(mtlfile);
		if(current_mtllib->Good() == false){
			std::cout << "ObjParser " << lineno << ": Problems opening mtllib " << mtlfile << std::endl;
			current_mtllib = nullptr;
		}else{
			current_mtllib->Parse();
		}
	}else if(split[0] == "usemtl"){
		StartNewMeshIfNotEmpty();
		if(current_mtllib == nullptr){
			std::cout << "ObjParser " << lineno << ": Ignoring usemtl because no material library has been loaded." << std::endl;
		}else{
			auto m = current_mtllib->GetMaterial(split[1]);
			if(m == nullptr){
				std::cout << "ObjParser " << lineno << ": Ignoring usemtl: material " << split[1] << " does not exist." << std::endl;
			}else{
				current_mesh->material = m;
			}
		}
	}else if(split[0] == "f"){
		if(split.size() <= 3  || split.size() >= 6){
			std::cout << "ObjParser " << lineno << ": Unsupported number of vetex entries in a face!" << std::endl;
			std::cout << line << std::endl;
			//return false;
		}
		if(split.size() == 4){
			face_vertex fv1 = ParseFW(split[1]);
			face_vertex fv2 = ParseFW(split[2]);
			face_vertex fv3 = ParseFW(split[3]);
			trig t = {fv1,fv2,fv3};
			t = FixMissingNormals(t);
			current_mesh->faces.push_back(t);
			mesh_nonempty = true;
		}else{ //5
			face_vertex fv1 = ParseFW(split[1]);
			face_vertex fv2 = ParseFW(split[2]);
			face_vertex fv3 = ParseFW(split[3]);
			face_vertex fv4 = ParseFW(split[4]);
			trig t1 = {fv1,fv2,fv3};
			trig t2 = {fv3,fv4,fv1};
			t1 = FixMissingNormals(t1);
			t2 = FixMissingNormals(t2);
			current_mesh->faces.push_back(t1);
			current_mesh->faces.push_back(t2);
			mesh_nonempty = true;
		}
	}else{
		std::cout << "ObjParser " << lineno << ": ignoring unknown entry '" << split[0] << "'" << std::endl;
	}

	return true;
}

face_vertex ObjParser::ParseFW(std::string s){
	int i1, i2, i3;
	auto split = SplitString(s,"/",false);
	if(split.size() == 1){
		i1 = std::stoi(split[0]);
		i2 = 0;
		i3 = 0;
	}else if(split.size() == 2){
		i1 = std::stoi(split[0]);
		i2 = std::stoi(split[1]);
		i3 = 0;
	}else if(split.size() == 3){
		i1 = std::stoi(split[0]);
		i2 = (split[1]=="")?0:std::stoi(split[1]);
		i3 = std::stoi(split[2]);
	}

	face_vertex res;
	if(i1 == 0) res.v = vert(0.0,0.0,0.0);
	if(i1 >  0) res.v = vertices[i1-1];
	if(i1 <  0) res.v = vertices[vertices.size() + i1];
	if(i2 == 0) res.vt = vert_tex(0.0,0.0);
	if(i2 >  0) res.vt = textures[i2-1];
	if(i2 <  0) res.vt = textures[textures.size() + i2];
	if(i3 == 0) res.vn = vert_normal(0.0,0.0,0.0);
	if(i3 >  0) res.vn = normals[i3-1];
	if(i3 <  0) res.vn = normals[normals.size() + i3];
	res.no_normal = (i3==0);
	return res;
}

inline trig ObjParser::FixMissingNormals(trig t){
	if(!t[0].no_normal && !t[1].no_normal && !t[2].no_normal) return t;
	glm::vec3 d1 = t[1].v - t[0].v;
	glm::vec3 d2 = t[2].v - t[0].v;
	glm::vec3 normal = glm::normalize(glm::cross(d1,d2));
	t[0].vn = t[1].vn = t[2].vn = normal;
	return t;
}

ObjParser::~ObjParser(){
	file.close();
}

MaterialLibrary::MaterialLibrary(std::string mtlname) : file(mtlname){
	path = mtlname;
	dir = GetDir(path);
}
MaterialLibrary::~MaterialLibrary(){
	file.close();
}

bool MaterialLibrary::Good(){
	return (bool)file;
}
void MaterialLibrary::Parse(){
	while(Step());
}

bool MaterialLibrary::Step(){
	std::string line;
	if(!std::getline(file, line)) return false;
	line.erase( std::remove_if(line.begin(), line.end(), [](char c){return c=='\r'||c=='\t';}), line.end() );
	auto split = SplitString(line, " ");
	if(split.size() < 1) return true;
	if(split[0][0] == '#'  || split[0][0] == '\n' || split[0][0] == '\r'){
		// a comment.
		return true;
	}else if(split[0] == "newmtl"){
		StartNewMaterial(split[1]);
	}else if(split[0] == "Ka"){
		current_material->ambient = mat_color(std::stof(split[1]),std::stof(split[2]),std::stof(split[3]));
	}else if(split[0] == "Kd"){
		current_material->diffuse = mat_color(std::stof(split[1]),std::stof(split[2]),std::stof(split[3]));
	}else if(split[0] == "Ks"){
		current_material->spectral = mat_color(std::stof(split[1]),std::stof(split[2]),std::stof(split[3]));
	}else if(split[0] == "Ns"){
		current_material->spectral_exponent = std::stof(split[1]);
	}else if(split[0] == "map_Ka"){
		std::replace( split[1].begin(), split[1].end(), '\\', '/');
		current_material->ambient_tex_path = dir + "/" + split[1];
	}else if(split[0] == "map_Kd"){
		std::replace( split[1].begin(), split[1].end(), '\\', '/');
		current_material->diffuse_tex_path = dir + "/" + split[1];
	}else if(split[0] == "map_Ks"){
		std::replace( split[1].begin(), split[1].end(), '\\', '/');
		current_material->spectral_tex_path = dir + "/" + split[1];
	}else if(split[0] == "Ni"){
		std::cout << "MtlParser: Ignoring refraction index." << std::endl;
	}else if(split[0] == "d" || split[0] == "Tr"){
		std::cout << "MtlParser: Ignoring transparency." << std::endl;
	}else if(split[0] == "illum"){
		std::cout << "MtlParser: Ignoring illumination mode." << std::endl;
	}else{
		std::cout << "MtlParser: ignoring unknown entry '" << split[0] << "'" << std::endl;
	}

	return true;
}

void MaterialLibrary::StartNewMaterial(std::string name){
	current_material = std::make_shared<Material>();
	material_map[name] = current_material;
	current_material->name = name;
}
std::shared_ptr<Material> MaterialLibrary::GetMaterial(std::string name){
	auto it = material_map.find(name);
	if(it == material_map.end()) return nullptr;
	return it->second;
}

std::vector<std::string> SplitString(std::string str, std::string delimiter, bool skipempty){
    std::vector<std::string> res;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        if( !(skipempty && token == "") ) res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if( !(skipempty && str == "") ) res.push_back(str);
    return res;
}
std::string GetDir(const std::string& str)
{
  	size_t found=str.find_last_of("/\\");
  	if(found == std::string::npos){
		return "";
	}
  	return str.substr(0,found);
}
