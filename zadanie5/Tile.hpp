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
	static GLuint positionsbuffer;
	static GLuint indicesbuffer[6];
	Tile();
	static inline int XYtoN(int x, int y) {return 1202*y+x;}
	static inline int XYtoNscaled(int x, int y, float no) {return 1202*int(1200*y/(no-1))+int(1200*x/(no-1));}
	inline short& AtN(int n);
	inline short& XYpos(int x, int y);
	inline short& XYnX(int x, int y);
	inline short& XYnY(int x, int y);
	void LoadFromHGTFile(std::ifstream& file, int lat, int lon);
	void GenerateNormals();
	static std::string LocateFile(int lat, int lon, std::string usercache, bool neverdownload = false);
	static bool TryDownload(std::string dir, int lat, int lon);
	static bool DownloadZIP(int lat, int lon);
	static void UnpackZIP(std::string path, std::string dir);
public:
	int lon, lat;
	// Initializes VBOs
	void Prepare();
	// Performs drawing
	unsigned int Render(short lod);

	// Prepares data shared between tiles
	static void Init();
	static int GetTileResolution(int lod);

	static std::shared_ptr<Tile> Create(int lat, int lon,std::string usercache, bool neverdownload = false);
	static std::shared_ptr<Tile> Create(std::string path, int lat, int lon);
	static std::string TileString(int lat, int lon);
	inline static std::string TileHGT(std::string dir, int lat, int lon);
};

#endif //TILE_HPP
