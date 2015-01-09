#define GLM_FORCE_RADIANS
#include "Render.hpp"
#include "Shader.hpp"
#include "Fonts.hpp"
#include "Text.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Viewpoint.hpp"

GLFWwindow* window;

// All the static members of Render class.
GLint Render::uniform_camera_transform, Render::uniform_perspective_transform;
GLuint Render::VertexArrayID;
float Render::pxsizex, Render::pxsizey;
GLuint Render::shader_program_id;
bool Render::inited = false;

double Render::GetTime(){
	return glfwGetTime();
}

bool Render::IsKeyPressed(int key){
	return glfwGetKey(window,key) == GLFW_PRESS;
}

bool Render::IsWindowClosed(){
	return (glfwWindowShouldClose(window) == 1);
}


int Render::Init(){

	// Initialise GLFW
	if( !glfwInit() ){
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1000, 1000, "Terrain", NULL, NULL);
	pxsizex = 2.0 / 1000;
	pxsizey = 2.0 / 1000;
	if( window == NULL ){
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Background
	glClearColor(50/255.0,80.0/255.0,0.7,1.0);

	// Prepare main vertex array.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	shader_program_id = LoadShaders( "TerrainVertexShader.vertexshader", "TerrainFragmentShader.fragmentshader" );

	// Prepare uniforms of the vertex shader
	uniform_camera_transform = glGetUniformLocation(shader_program_id, "camera_transform");
	uniform_perspective_transform = glGetUniformLocation(shader_program_id, "perspective_transform");
	if(uniform_camera_transform == -1 || uniform_perspective_transform == -1){
		std::cerr << "A uniform is missing from the shader." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Enable blending for several alpha effects.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Prepare for rendering fonts.
	int res = init_font();
	if(res < 0) return res;

	inited = true;
	return 0;
}

glm::vec2 Render::ProbeMouse(){
	double x,y;
	glfwGetCursorPos(window,&x,&y);
	glfwSetCursorPos(window,500.0,500.0);
	return glm::vec2((x-500)/500,(y-500)/500);
}


void Render::FrameStart(){
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the board shader
	glUseProgram(shader_program_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	/*
	// Prepare lights
	int n = Light::lights.size();
	if(Light::lights.size() > 0){
		float lightspos[3*n], lightparrams[2*n], lightcolors[3*n], sda[3*n], fixranges[n];
		glUniform1i(uniform_lightnumber,n);
		for(int i = 0; i < n; i++){
			Light* l = Light::lights[i];
			glm::vec3 lightpos(l->GetGlobalPos());
			lightspos[3*i+0] = lightpos.x;
			lightspos[3*i+1] = lightpos.y;
			lightspos[3*i+2] = lightpos.z;
			lightcolors[3*i+0] = l->color.x * l->multiplier;
			lightcolors[3*i+1] = l->color.y * l->multiplier;
			lightcolors[3*i+2] = l->color.z * l->multiplier;
			lightparrams[2*i+0] = l->distance_influence;
			lightparrams[2*i+1] = l->spatial_range;
			sda[3*i+0] = l->sda.x;
			sda[3*i+1] = l->sda.y;
			sda[3*i+2] = l->sda.z;
			fixranges[i] = l->fixrange;
		}
		glUniform3fv(uniform_lightpos,n,lightspos);
		glUniform2fv(uniform_lightparrams,n,lightparrams);
		glUniform3fv(uniform_lightcolor,n,lightcolors);
		glUniform3fv(uniform_light_sda,n,sda);
		glUniform1fv(uniform_light_fixrange,n,fixranges);
	}
	*/

	// Prepare camera
	if(Viewpoint::active_viewpoint){
		glm::mat4 cameraview =  glm::lookAt(glm::vec3(0.0) , 1.0f* Viewpoint::active_viewpoint->GetDirection(), glm::vec3(0.0,0.0,1.0)) * glm::inverse(Viewpoint::active_viewpoint->GetTransform());
		glm::mat4 perspective = glm::perspective(Viewpoint::active_viewpoint->GetFOV(), 1.0f, 0.005f, 10.0f);
		glUniformMatrix4fv(uniform_camera_transform, 1, GL_FALSE, &cameraview[0][0]);
		glUniformMatrix4fv(uniform_perspective_transform, 1, GL_FALSE, &perspective[0][0]);
	}

}

void Render::FrameEnd(){
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	for(auto t : Text::texts){
		if(!t->active) continue;
		glm::vec2 off = t->px_offset;
		glm::vec3 color = t->color;

		render_text(t->text.c_str(), -1.0 + off.x*pxsizex, 1.0 - off.y*pxsizey, color.r, color.g, color.b, t->size, pxsizex, pxsizey);
	}

	glUseProgram(0);

	// Swap buffers
	glfwSwapBuffers(window);
	// Check for events
	glfwPollEvents();
}

void Render::CleanUp(){
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shader_program_id);

	// Close the window.
	glfwTerminate();
}
