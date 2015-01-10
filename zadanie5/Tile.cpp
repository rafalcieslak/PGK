#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include "Tile.hpp"
#include "Render.hpp"

GLuint Tile::positionsbuffer;
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define RESTART_INDEX ((unsigned int)-1)
#define HSCALE (1.0/15000.0)

int LODs[] = {1201, 600, 400, 250, 100, 30};
GLuint Tile::indicesbuffer[6];

Tile::Tile(){
}

void Tile::Init(){
    std::vector<unsigned int> positions(1201*1202);
    for(int y = 0; y < 1201; y++)
        for(int x = 0; x < 1201; x++)
            positions[XYtoN(x,y)] = (x<<16) | y;

    glGenBuffers(1, &positionsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionsbuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(unsigned int), positions.data(), GL_STATIC_DRAW);
    glGenBuffers(6, indicesbuffer);
    for(int i = 0; i < 6; i++){
        std::vector<unsigned int> indices;
        int n = LODs[i];
        indices.reserve(n*(n+1)*2);
        for(int y = 0; y < n; y++){
            for(int x = 0; x < n; x++){
                indices.push_back(XYtoNscaled(x,y,n   ));
                indices.push_back(XYtoNscaled(x,y+1,n ));
            }
            indices.push_back(RESTART_INDEX);
            indices.push_back(RESTART_INDEX);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }
}

std::shared_ptr<Tile> Tile::Create(int lat, int lon){
    PrepareFile(lat,lon);
    std::ifstream file(TileHGT(lat,lon), std::ios::in|std::ios::binary);
	if(!file.is_open()) return nullptr;
	std::shared_ptr<Tile> t = std::shared_ptr<Tile>(new Tile());
    t->LoadFromHGTFile(file, lat, lon);
	file.close();
    t->GenerateNormals();
	return t;
}

void Tile::LoadFromHGTFile(std::ifstream& file, int lat_, int lon_){
    std::cout << "Loading DTED for tile " << lat_ << " " << lon_ << std::endl;
    for(int y = 0; y < 1201; y++){
        for(int x = 0; x < 1201; x++){
            char data[2];
            file.read(data,2);
            short a0 = data[0];
            short a1 = data[1]&0x00ff;
            XYpos(x,y) = (a0 << 8) | a1;
        }
    }
    lat = lat_;
    lon = lon_;
}

inline bool exists(std::string filename){
    std::ifstream f(filename.c_str());
    if(f.good()){
        f.close(); return true;
    }else{
        f.close(); return false;
    }
}

void Tile::PrepareFile(int lat, int lon){
    std::cout << "Preparing file for tile " << lat << " " << lon << std::endl;
    if(!exists(TileHGT(lat,lon))){
        if(!exists(TileHGT(lat,lon) + ".zip")){
            DownloadZIP(lat,lon);
        }
        UnpackZIP(lat,lon);
    }
}
bool Tile::TryDownload(std::string dir, int lat, int lon){
    std::string url = "http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/" + dir + "/" + TileString(lat,lon) + ".hgt.zip";
    std::string path = TileHGT(lat,lon) + ".zip";
    std::cout << "Trying to download " << url << " with wget" << std::endl;
    std::string command = "wget " + url + " --quiet -O " + path;
    int result = system(command.c_str());
    if(result == 8){
        std::cout << "Server issued an HTTP error response." << std::endl;
        return false;
    }else if(result != 0){
        std::cout << "Non-HTTP error." << std::endl;
        return false;
    }
    return true;
}
bool Tile::DownloadZIP(int lat, int lon){
    if(!TryDownload("Eurasia", lat, lon))
        if(!TryDownload("Africa", lat, lon))
            if(!TryDownload("North America", lat, lon))
                if(!TryDownload("South America", lat, lon))
                    if(!TryDownload("Australia", lat, lon))
                        if(!TryDownload("Islands", lat, lon))
                            return false; //pass
    return true;
}
void Tile::UnpackZIP(int lat, int lon){
    std::string command = "unzip -o " + TileHGT(lat,lon) + ".zip -d /tmp/";
    system(command.c_str());
}

void Tile::GenerateNormals(){
    std::cout << "Calculating normals for tile " << lat << " " << lon << std::endl;
    for(int y = 0; y < 1201; y++){
        for(int x = 0; x < 1201; x++)
        {
            // x normals.
            int h0, h1, h2;
            h1 = XYpos(x,y);
            if(x == 0) h0 = h1; else h0 = XYpos(x-1,y);
            if(x == 1200) h2 = h1; else h2 = XYpos(x+1,y);
            float a = (h0-h1);
            float b = (h1-h2);
            XYnX(x,y) = -(a+b)/2.0;
            // y normals
            h1 = XYpos(x,y);
            if(y == 0) h0 = h1; else h0 = XYpos(x,y-1);
            if(y == 1200) h2 = h1; else h2 = XYpos(x,y+1);
            a = (h0-h1);
            b = (h1-h2);
            XYnY(x,y) = -(a+b)/2.0;
        }
    }
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

std::string Tile::TileHGT(int lat, int lon){
    return "/tmp/" + Tile::TileString(lat,lon) + ".hgt";
}

void Tile::Prepare(){
    glGenBuffers(1, &databuffer);
    glBindBuffer(GL_ARRAY_BUFFER, databuffer);
    glBufferData(GL_ARRAY_BUFFER, 1201*1202*3 * sizeof(short), data.data(), GL_STATIC_DRAW);
}

void Tile::Render(short lod){

    glUniform2f(Render::uniform_pos, (float)lon, (float)lat );
    glBindBuffer(GL_ARRAY_BUFFER, positionsbuffer);
    glVertexAttribIPointer( 0, 1, GL_UNSIGNED_INT, 0, (void*)0 );
    glBindBuffer(GL_ARRAY_BUFFER, databuffer);
    glVertexAttribIPointer( 1, 1, GL_SHORT, 3*sizeof(short), (void*)0 );
    glVertexAttribIPointer( 2, 1, GL_SHORT, 3*sizeof(short), (void*)2 );
    glVertexAttribIPointer( 3, 1, GL_SHORT, 3*sizeof(short), (void*)4 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer[lod]);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_INDEX);
    glDrawElements(GL_TRIANGLE_STRIP, LODs[lod]*LODs[lod]*2, GL_UNSIGNED_INT, (void*)0 );
}
