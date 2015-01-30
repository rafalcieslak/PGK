#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "TextureLoader.hpp"

std::map<std::pair<std::string, std::string>,GLuint> loaded_textures;

GLuint LoadBMP(const char * imagepath);
GLuint LoadDDS(const char * imagepath);

extern bool auto_convert;

inline bool exists(std::string filename){
	std::ifstream f(filename.c_str());
	if(f.good()){
		f.close(); return true;
	}else{
		f.close(); return false;
	}
}
void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

GLuint GetTexture(std::string dir, std::string path){
	auto it = loaded_textures.find(std::make_pair(dir,path));
	if(it != loaded_textures.end()){
		return it->second;
	}
	std::string ext = path.substr(path.size()-3);
	GLuint id = GL_INVALID_VALUE;

	std::string fullpath = dir + "/" + path;

	if(path[0] == '/' || (path[1] == ':' && path[2] == '/')){
		std::cout << "Texture path " << path << " looks like an incorrectly exported absolute path." << std::endl;
		size_t pos = path.find_last_of('/')+1;
		fullpath = dir + "/" + path.substr(pos);
		std::cout << "  Will try " << fullpath << " instead." << std::endl;
	}


	if(exists(fullpath)){
		if(ext == "bmp" || ext == "BMP"){
			id = LoadBMP(fullpath.c_str());
		}else if(ext == "dds" || ext == "DDS"){
			id = LoadDDS(fullpath.c_str());
		}else{
			std::cout << "Unable to load texture " << fullpath << ": unsupported format" << std::endl;
			std::string alternative_path = fullpath.substr(0,fullpath.size()-3) + "bmp";
			if(exists(alternative_path)){
				id = LoadBMP(alternative_path.c_str());
			}else{
				if(!auto_convert){
					std::cout << " ----- Texture format " << fullpath.substr(fullpath.size()-3) << " is not supported, but if you convert " << fullpath << " to " << alternative_path << " (24bpp) it will be used instead." << std::endl;
					std::cout << " ----- For example, you may run `convert +matte " << fullpath << " " << alternative_path << "`" << std::endl;
				}else{
					// perform autoconversion and retry
					std::string p1 = fullpath, p2 = alternative_path;
					ReplaceAll(p1," ","\\ ");
					ReplaceAll(p2," ","\\ ");
					std::string command = "convert +matte " + p1 + " " + p2;
					std::cout << "Running conversion: " << command << std::endl;
					system(command.c_str());
					id = LoadBMP(alternative_path.c_str());
				}
			}
		}
	}else{
		std::cout << "Unable to load " << fullpath << " - file does not exist!" << std::endl;
	}
	if(id != GL_INVALID_VALUE) std::cout << "Loaded texture: " << fullpath << std::endl;
	loaded_textures[std::make_pair(dir,path)] = id;
	return id;
}


// Code below was borrowed from the tutorial

GLuint LoadBMP(const char * imagepath){

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("%s could not be opened.\n", imagepath); return GL_INVALID_VALUE;}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){
		printf("Not a correct BMP file\n");
		return GL_INVALID_VALUE;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return GL_INVALID_VALUE;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return GL_INVALID_VALUE;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return GL_INVALID_VALUE;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint LoadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened.\n", imagepath);
		return GL_INVALID_VALUE;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return GL_INVALID_VALUE;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	//unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return GL_INVALID_VALUE;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width  /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	}

	free(buffer);

	return textureID;


}
