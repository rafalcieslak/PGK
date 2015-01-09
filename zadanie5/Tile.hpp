#ifndef TILE_HPP
#define TILE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <memory>

class Tile{
private:
	std::array<short,1201*1201> data;
	GLuint databuffer;
	static GLuint indicesbuffer, positionsbuffer;
	Tile();
	static inline int XYtoN(int x, int y) {return 1201*y+x;}
	inline short& AtN(int n);
	inline short& AtXY(int x, int y);
public:
	void Prepare();
	void Render();
	static void Init();

	static std::shared_ptr<Tile> CreateFromHGTFile(std::string hgtfile);
	static std::string TileString(int lat, int lon);
};

#endif //TILE_HPP
