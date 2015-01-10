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
	static void PrepareFile(int lat, int lon);
	static bool TryDownload(std::string dir, int lat, int lon);
	static bool DownloadZIP(int lat, int lon);
	static void UnpackZIP(int lat, int lon);
public:
	int lon, lat;
	void Prepare();
	void Render();
	static void Init();

	static std::shared_ptr<Tile> Create(int lat, int lon);
	static std::string TileString(int lat, int lon);
	inline static std::string TileHGT(int lat, int lon) __attribute__((pure));
};

#endif //TILE_HPP
