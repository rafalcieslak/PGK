#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include "Tile.hpp"

GLuint Tile::positionsbuffer, Tile::indicesbuffer;

Tile::Tile(){
    glGenBuffers(1, &databuffer);
}

void Tile::Init(){
    std::vector<unsigned int> positions(1201*1201);
    for(int y = 0; y < 1201; y++)
        for(int x = 0; x < 1201; x++)
            positions[y*1201+x] = (x<<16) | y;

    glGenBuffers(1, &positionsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionsbuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(unsigned int), positions.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(unsigned int), positions.data(), GL_STATIC_DRAW);
    std::vector<unsigned int> indices;
    indices.reserve(1200*1200*6);
    for(int y = 0; y < 1200; y++)
        for(int x = 0; x < 1200; x++){
            indices.push_back(y*1201 + x);
            indices.push_back(y*1201 + x+1);
            indices.push_back((y+1)*1201 + x);
            indices.push_back((y+1)*1201 + x+1);
            indices.push_back((y+1)*1201 + x);
            indices.push_back(y*1201 + x+1);
        }
    glGenBuffers(1, &indicesbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

std::shared_ptr<Tile> Tile::CreateFromHGTFile(std::string hgtfile){
    std::ifstream file(hgtfile, std::ios::in|std::ios::binary);
    std::cout << hgtfile << std::endl;
	if(!file.is_open()) return nullptr;
	std::shared_ptr<Tile> t = std::shared_ptr<Tile>(new Tile());
	for(int y = 0; y < 1201; y++)
		for(int x = 0; x < 1201; x++)
		{
            char data[2];
			file.read(data,2);
            short a0 = data[0];
            short a1 = data[1]&0x00ff;
			t->AtXY(x,y) = (a0 << 8) | a1;
            //if(t->AtXY(x,y) < 0 && t->AtXY(x,y) != -32768) assert(false);
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
	ss << std::setw(3) << std::setfill('0') << lon;
	return ss.str();
}

void Tile::Prepare(){
    glBindBuffer(GL_ARRAY_BUFFER, databuffer);
    glBufferData(GL_ARRAY_BUFFER, 1201*1201 * sizeof(short), data.data(), GL_STATIC_DRAW);
}

void Tile::Render(){

    glBindBuffer(GL_ARRAY_BUFFER, positionsbuffer);
    glVertexAttribIPointer( 0, 1, GL_UNSIGNED_INT, 0, (void*)0 );
    glBindBuffer(GL_ARRAY_BUFFER, databuffer);
    glVertexAttribIPointer( 1, 1, GL_SHORT, 0, (void*)0 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer);
    glDrawElements(GL_TRIANGLES, 1200*1200*6, GL_UNSIGNED_INT, (void*)0 );
}
