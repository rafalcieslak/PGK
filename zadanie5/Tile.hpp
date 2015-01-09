#ifndef TILE_HPP
#define TILE_HPP

#include <array>
#include <string>
#include <memory>

class Tile{
private:
	std::array<short,1201*1201> data;
	Tile();
	static inline int XYtoN(int x, int y) {return 1201*x+y;}
	inline short& AtN(int n);
	inline short& AtXY(int x, int y);
public:

	static std::shared_ptr<Tile> CreateFromHGTFile(std::string hgtfile);
	static std::string TileString(int lat, int lon);
};

#endif //TILE_HPP
