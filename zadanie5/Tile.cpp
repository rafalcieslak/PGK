#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include "Tile.hpp"

GLuint Tile::positionsbuffer, Tile::indicesbuffer;
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define RESTART_INDEX ((unsigned int)-1)
#define HSCALE (1.0/15000.0)

Tile::Tile(){
    glGenBuffers(1, &databuffer);
}

void Tile::Init(){
    std::vector<unsigned int> positions(1201*1202);
    for(int y = 0; y < 1201; y++)
        for(int x = 0; x < 1201; x++)
            positions[XYtoN(x,y)] = (x<<16) | y;

    glGenBuffers(1, &positionsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionsbuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(unsigned int), positions.data(), GL_STATIC_DRAW);
    std::vector<unsigned int> indices;
    indices.reserve(1201*1202*2);
    for(int y = 0; y < 1201; y++){
        for(int x = 0; x < 1201; x++){
            indices.push_back(XYtoN(x,y));
            indices.push_back(XYtoN(x,y+1));
        }
        indices.push_back(RESTART_INDEX);
        indices.push_back(RESTART_INDEX);
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
	for(int y = 0; y < 1201; y++){
		for(int x = 0; x < 1201; x++)
		{
            char data[2];
			file.read(data,2);
            short a0 = data[0];
            short a1 = data[1]&0x00ff;
			t->XYpos(x,y) = (a0 << 8) | a1;
		}
    }
    // Generate normals.

    for(int y = 0; y < 1201; y++){
        for(int x = 0; x < 1201; x++)
        {
            // x normals.
            int h0, h1, h2;
            h1 = t->XYpos(x,y);
            if(x == 0) h0 = h1; else h0 = t->XYpos(x-1,y);
            if(x == 1200) h2 = h1; else h2 = t->XYpos(x+1,y);
            float a = (h0-h1);
            float b = (h1-h2);
            t->XYnX(x,y) = -(a+b)/2.0;
            // y normals
            h1 = t->XYpos(x,y);
            if(y == 0) h0 = h1; else h0 = t->XYpos(x,y-1);
            if(y == 1200) h2 = h1; else h2 = t->XYpos(x,y+1);
            a = (h0-h1);
            b = (h1-h2);
            t->XYnY(x,y) = -(a+b)/2.0;
        }
    }
	file.close();
	return t;
}

short& Tile::AtN(int n){
	return data[n];
}

short& Tile::XYpos(int x, int y){
	return data[XYtoN(x,y)*3];
}
short& Tile::XYnX(int x, int y){
	return data[XYtoN(x,y)*3+1];
}
short& Tile::XYnY(int x, int y){
	return data[XYtoN(x,y)*3+2];
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
    glBufferData(GL_ARRAY_BUFFER, 1201*1202*3 * sizeof(short), data.data(), GL_STATIC_DRAW);
}

void Tile::Render(){

    glBindBuffer(GL_ARRAY_BUFFER, positionsbuffer);
    glVertexAttribIPointer( 0, 1, GL_UNSIGNED_INT, 0, (void*)0 );
    glBindBuffer(GL_ARRAY_BUFFER, databuffer);
    glVertexAttribIPointer( 1, 1, GL_SHORT, 3*sizeof(short), (void*)0 );
    glVertexAttribIPointer( 2, 1, GL_SHORT, 3*sizeof(short), (void*)2 );
    glVertexAttribIPointer( 3, 1, GL_SHORT, 3*sizeof(short), (void*)4 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_INDEX);
    glDrawElements(GL_TRIANGLE_STRIP, 1201*1201*2, GL_UNSIGNED_INT, (void*)0 );
}
