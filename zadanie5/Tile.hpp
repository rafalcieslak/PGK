#ifndef TILE_HPP
#define TILE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <memory>
#include <fstream>

class Tile{
private:
	std::array<short,1201*1202*3> data;
	GLuint databuffer;
	static GLuint indicesbuffer, positionsbuffer;
	Tile();
	static inline int XYtoN(int x, int y) {return 1202*y+x;}
	inline short& AtN(int n);
	inline short& XYpos(int x, int y);
	inline short& XYnX(int x, int y);
	inline short& XYnY(int x, int y);
	void LoadFromHGTFile(std::ifstream& file, int lat, int lon);
	void GenerateNormals();
public:
	int lon, lat;
	void Prepare();
	void Render();
	static void Init();

	static std::shared_ptr<Tile> CreateFromHGTFile(int lat, int lon);
	static std::string TileString(int lat, int lon);
};

#endif //TILE_HPP
