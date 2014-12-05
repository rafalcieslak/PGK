#include "Bubble.hpp"
#include "../engine/ModelBase.hpp"
#include <cmath>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include "main.hpp"

#define rgb(x,y,z)	x/255.0, y/255.0, z/255.0, 1.0
#define rgba(x,y,z,a)	x/255.0, y/255.0, z/255.0, a
#define C_BLUE rgb(58, 92, 226)
#define C_RED rgb(223, 41, 31)
#define C_GREEN rgb(45, 199, 81)
#define BUBBLE_BLUE rgba(75, 165, 225, 0.1)
#define x3(a) a,a,a
#define x3b(a,b,c) a,b,c,a,b,c,a,b,c
#define x12(a) a,a,a,a,a,a,a,a,a,a,a,a
#define x4(a,b,c) a,b,c,a,b,c,a,b,c,a,b,c

inline void add_triangle(std::vector<float>& v, const glm::vec3 a, const glm::vec3 b, const glm::vec3 c){
	v.push_back(a.x); v.push_back(a.y); v.push_back(a.z);
	v.push_back(b.x); v.push_back(b.y); v.push_back(b.z);
	v.push_back(c.x); v.push_back(c.y); v.push_back(c.z);
}
inline void add_triangle_color(std::vector<float>& v, const glm::vec4 a, const glm::vec4 b, const glm::vec4 c){
	v.push_back(a.x); v.push_back(a.y); v.push_back(a.z); v.push_back(a.w);
	v.push_back(b.x); v.push_back(b.y); v.push_back(b.z); v.push_back(b.w);
	v.push_back(c.x); v.push_back(c.y); v.push_back(c.z); v.push_back(c.w);
}
inline void add_triangle_REV(std::vector<float>& v, const glm::vec3 a, const glm::vec3 b, const glm::vec3 c){
	v.push_back(c.x); v.push_back(c.y); v.push_back(c.z);
	v.push_back(b.x); v.push_back(b.y); v.push_back(b.z);
	v.push_back(a.x); v.push_back(a.y); v.push_back(a.z);
}
inline void add_triangle_color_REV(std::vector<float>& v, const glm::vec4 a, const glm::vec4 b, const glm::vec4 c){
	v.push_back(c.x); v.push_back(c.y); v.push_back(c.z); v.push_back(c.w);
	v.push_back(b.x); v.push_back(b.y); v.push_back(b.z); v.push_back(b.w);
	v.push_back(a.x); v.push_back(a.y); v.push_back(a.z); v.push_back(a.w);
}

int prepare_ball_model(const unsigned int hstripes, const unsigned int ncir, std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& colors){
	vertices.clear(); normals.clear(); colors.clear();
	int n = 0;
	glm::vec4 color(BUBBLE_BLUE);

	//float hangle = 1.0f/hstripes;
	const float rangle = 1.0f/ncir;

	// lower half
	for(unsigned int stripeno = 0; stripeno < hstripes; stripeno++){
		float hlow = sin(stripeno*0.5*M_PI/hstripes);
		float slow = cos(stripeno*0.5*M_PI/hstripes);
		float hhigh = sin((stripeno+1)*0.5*M_PI/hstripes);
		float shigh = cos((stripeno+1)*0.5*M_PI/hstripes);
		for(unsigned int i = 0; i < ncir; i++){
			float angleA = (i      + 0.5*(stripeno%2))*rangle;
			float angleB = (i+0.5f + 0.5*(stripeno%2))*rangle;
			float angleC = (i+1    + 0.5*(stripeno%2))*rangle;
			float angleD = (i+1.5f + 0.5*(stripeno%2))*rangle;
			glm::vec3 pointA = glm::vec3( slow*sin(angleA*2.0*M_PI), -hlow, -slow*cos(angleA*2.0*M_PI));
			glm::vec3 pointB = glm::vec3(shigh*sin(angleB*2.0*M_PI),-hhigh,-shigh*cos(angleB*2.0*M_PI));
			glm::vec3 pointC = glm::vec3( slow*sin(angleC*2.0*M_PI), -hlow, -slow*cos(angleC*2.0*M_PI));
			glm::vec3 pointD = glm::vec3(shigh*sin(angleD*2.0*M_PI),-hhigh,-shigh*cos(angleD*2.0*M_PI));
			glm::vec3 normalA = (2.0f*pointA - pointA); // will be of unit length
			glm::vec3 normalB = (2.0f*pointB - pointB);
			glm::vec3 normalC = (2.0f*pointC - pointC);
			glm::vec3 normalD = (2.0f*pointD - pointD);
			add_triangle_REV(vertices, pointA, pointB, pointC);
			add_triangle_REV(vertices, pointC, pointB, pointD);
			add_triangle_REV(normals, normalA, normalB, normalC);
			add_triangle_REV(normals, normalC, normalB, normalD);
			add_triangle_color_REV(colors, color, color, color);
			add_triangle_color_REV(colors, color, color, color);
			n += 2;
		}
	}
	// upper half
	for(unsigned int stripeno = 0; stripeno < hstripes; stripeno++){
		float hlow = sin(stripeno*0.5*M_PI/hstripes);
		float slow = cos(stripeno*0.5*M_PI/hstripes);
		float hhigh = sin((stripeno+1)*0.5*M_PI/hstripes);
		float shigh = cos((stripeno+1)*0.5*M_PI/hstripes);
		for(unsigned int i = 0; i < ncir; i++){
			float angleA = (i      + 0.5*(stripeno%2))*rangle;
			float angleB = (i+0.5f + 0.5*(stripeno%2))*rangle;
			float angleC = (i+1    + 0.5*(stripeno%2))*rangle;
			float angleD = (i+1.5f + 0.5*(stripeno%2))*rangle;
			glm::vec3 pointA = glm::vec3( slow*sin(angleA*2.0*M_PI), hlow, -slow*cos(angleA*2.0*M_PI));
			glm::vec3 pointB = glm::vec3(shigh*sin(angleB*2.0*M_PI),hhigh,-shigh*cos(angleB*2.0*M_PI));
			glm::vec3 pointC = glm::vec3( slow*sin(angleC*2.0*M_PI), hlow, -slow*cos(angleC*2.0*M_PI));
			glm::vec3 pointD = glm::vec3(shigh*sin(angleD*2.0*M_PI),hhigh,-shigh*cos(angleD*2.0*M_PI));
			glm::vec3 normalA = (2.0f*pointA - pointA); // will be of unit length
			glm::vec3 normalB = (2.0f*pointB - pointB);
			glm::vec3 normalC = (2.0f*pointC - pointC);
			glm::vec3 normalD = (2.0f*pointD - pointD);
			add_triangle(vertices, pointA, pointB, pointC);
			add_triangle(vertices, pointC, pointB, pointD);
			add_triangle(normals, normalA, normalB, normalC);
			add_triangle(normals, normalC, normalB, normalD);
			add_triangle_color(colors, color, color, color);
			add_triangle_color(colors, color, color, color);
			n += 2;
		}
	}
	return n;
}

Bubble::Bubble(float scale){
	if(!ModelBase::GetInstance().HasModel("ball")){
		std::vector<float> vertices, normals, colors;
		int n = prepare_ball_model(16,32,vertices,normals,colors);
		std::cerr << "Ball model has " << n << " triangles." << std::endl;
		ModelBase::GetInstance().AddModelTriangles("ball",n, vertices.data(), normals.data(), colors.data());
	}
	model_id = "ball";
	SetScale(scale);
}

void Bubble::RotateTowards(glm::vec3 to){
	glm::vec4 my_up =  glm::vec4(0.0,1.0,0.0,1.0);
	glm::vec4 my_pos = GetGlobalTransform() * glm::vec4(0.0,0.0,0.0,1.0);
	glm::vec4 dir = glm::vec4(to,1.0) - my_pos;
	glm::quat rot = glm::rotation(glm::normalize(my_up.xyz()), glm::normalize(dir.xyz()));
	SetRotation(rot);
}

void Bubble::ApplyMovement(float time_delta){
	glm::vec3 pos = GetPosition();
	float my_h = ZPosToH(pos.z);
	float new_h = my_h + SpeedFunc(my_h)*time_delta; // this should be an integral...
	/*std::cout << "z " << pos.z << std::endl;
	std::cout << "bubble h " << my_h << std::endl;
	std::cout << "bubble newh " << new_h << std::endl;
	std::cout << "bubble newz " << HToZPos(new_h) << std::endl;*/
	SetPosition(pos.x,pos.y,HToZPos(new_h));
	SetScale(BUBBLE_MAIN_SIZE*ScaleFunc(new_h));
}

float Bubble::ZPosToH(float zpos){
	float a = zpos/(ROOM_SIZE_Z/2.0);
	return (a+1.0)/2.0;
}
float Bubble::HToZPos(float h){
	float a = (h*2.0)-1.0;
	return a*ROOM_SIZE_Z/2.0;
}
float Bubble::SpeedFunc(float h){
	return h*h*h/2.0 + 0.05;
}
float Bubble::ScaleFunc(float h){
	return h*h/0.9 + 0.2;
}
bool Bubble::ShouldPop(){
	return ZPosToH(GetPosition().z) >= 1.0;
}
