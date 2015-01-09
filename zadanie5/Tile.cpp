#include <fstream>
#include <sstream>
#include <iomanip>
#include "Tile.hpp"

Tile::Tile(){

}

std::shared_ptr<Tile> Tile::CreateFromHGTFile(std::string hgtfile){
    std::ifstream file(hgtfile, std::ios::in|std::ios::binary);
	if(!file.is_open()) return nullptr;
	std::shared_ptr<Tile> t = std::shared_ptr<Tile>(new Tile());
	for(int x = 0; x < 1201; x++)
		for(int y = 0; y < 1201; y++)
		{
			char data[2];
			file.read(data,2);
			t->AtXY(x,y) = (data[1] << 8) | data[0];
		}
	file.close();
	return t;
}

short& Tile::AtN(int n){
	return data[n];
}

short& Tile::AtXY(int x, int y){
	return data[XYtoN(x,y)];
}

std::string Tile::TileString(int lat, int lon){
	std::stringstream ss;
	if(lat < 0) ss << "S";
	else        ss << "N";
	ss << std::setw(2) << std::setfill('0') << lat;
	if(lon < 0) ss << "W";
	else        ss << "E";
	ss << std::setw(2) << std::setfill('0') << lon;
	return ss.str();
}
