#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include "Tile.hpp"
#include <sys/wait.h>
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

std::shared_ptr<Tile> Tile::Create(int lat, int lon, std::string usercache){
    std::string path = LocateFile(lat,lon,usercache);
    std::ifstream file(path, std::ios::in|std::ios::binary);
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
std::ifstream::pos_type filesize(std::string filename)
{
    std::ifstream f(filename, std::ifstream::ate | std::ifstream::binary);
    return f.tellg();
}

std::string Tile::LocateFile(int lat, int lon, std::string usercache){
    std::string userpath = TileHGT(usercache,lat,lon);
    std::string  tmppath = TileHGT("/tmp",  lat,lon);
    if(usercache != ""){
        std::cout << "Looking for tile " << TileString(lat,lon) << " in user cache: " << userpath << "...";
        if(exists(userpath)){
            std::cout << "Found!" << std::endl;
            return userpath;
        }
        std::cout << "Not found." << std::endl;
    }
    std::cout << "Looking for tile " << TileString(lat,lon) << " in /tmp cache: " << tmppath << "...";
    if(exists(tmppath)){
        std::cout << "Found!" << std::endl;
        return tmppath;
    }
    std::cout << "Not found." << std::endl;
    if(!exists(tmppath + ".zip")){
        DownloadZIP(lat,lon);
    }
    if(filesize(tmppath + ".zip") < 1){
        std::cout << "This tile is empty." << std::endl;
        return "";
    }
    std::cout << "Unzipping " << tmppath << ".zip" << std::endl;
    UnpackZIP(tmppath, "/tmp/");
    return tmppath;
}
bool Tile::TryDownload(std::string dir, int lat, int lon){
    std::string url = "http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/" + dir + "/" + TileString(lat,lon) + ".hgt.zip";
    std::string path = TileHGT("/tmp",lat,lon) + ".zip";
    std::cout << "Trying to download " << url << " with wget" << std::endl;
    std::string command = "wget " + url + " --quiet -O " + path;
    int q = system(command.c_str());
    int result = WEXITSTATUS(q);
    if(result == 8){
        std::cout << "Server issued an HTTP error response. Probably the file does not exist on the server." << std::endl;
        return false;
    }else if(result != 0){
        if(result == 2){
            // interrupted! delete the partially-downloaded file
            command = "rm " + path;
            q = system(command.c_str());
            exit(0);
        }
        std::cout << "Wget exit status: " << result << std::endl;
        return false;
    }
    return true;
}
bool Tile::DownloadZIP(int lat, int lon){
    if(!TryDownload("Eurasia", lat, lon))
        if(!TryDownload("Africa", lat, lon))
            if(!TryDownload("North\\ America", lat, lon))
                if(!TryDownload("South\\ America", lat, lon))
                    if(!TryDownload("Australia", lat, lon))
                        if(!TryDownload("Islands", lat, lon))
                            return false; //pass
    return true;
}
void Tile::UnpackZIP(std::string tmppath, std::string tmpdir){
    std::string command = "unzip -o " + tmppath + ".zip -d " + tmpdir + " > /dev/null 2> /dev/null";
    int q = system(command.c_str());
    if(q != 0){
        std::cout << "There were errors while extracting " << tmppath << std::endl;
    }
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
            short res = -(a+b)/2.0;
            if(res > 1000 || res < -1000 ) res = 0;
            XYnX(x,y) = res;
            // y normals
            h1 = XYpos(x,y);
            if(y == 0) h0 = h1; else h0 = XYpos(x,y-1);
            if(y == 1200) h2 = h1; else h2 = XYpos(x,y+1);
            a = (h0-h1);
            b = (h1-h2);
            res = -(a+b)/2.0;
            if(res > 1000 || res < -1000 ) res = 0;
            XYnY(x,y) = res;
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

int Tile::GetTileResolution(int lod){
    return LODs[lod];
}

std::string Tile::TileString(int lat, int lon){
	std::stringstream ss;
	if(lat < 0){
        ss << "S";
    	ss << std::setw(2) << std::setfill('0') << -lat;
	}else{
        ss << "N";
    	ss << std::setw(2) << std::setfill('0') << lat;
    }
	if(lon < 0){
        ss << "W";
    	ss << std::setw(3) << std::setfill('0') << -lon;
	}else{
        ss << "E";
    	ss << std::setw(3) << std::setfill('0') << lon;
    }
	return ss.str();
}

std::string Tile::TileHGT(std::string dir, int lat, int lon){
    return dir + "/" + Tile::TileString(lat,lon) + ".hgt";
}

void Tile::Prepare(){
    glGenBuffers(1, &databuffer);
    glBindBuffer(GL_ARRAY_BUFFER, databuffer);
    glBufferData(GL_ARRAY_BUFFER, 1201*1202*3 * sizeof(short), data.data(), GL_STATIC_DRAW);
}

unsigned int Tile::Render(short lod){

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
    return LODs[lod]*LODs[lod]*2;
}
