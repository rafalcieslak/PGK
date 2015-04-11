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
GLint Render::uniform_camera_transform_g, Render::uniform_perspective_transform_g;
GLint Render::uniform_pos, Render::uniform_xscale, Render::uniform_xscale_g;
GLint Render::uniform_light_intensity, Render::uniform_terrainscale;
GLint Render::uniform_light_angle, Render::uniform_sphere_g, Render::uniform_sphere;
GLuint Render::VertexArrayID;
float Render::pxsizex, Render::pxsizey;
GLuint Render::shader_program_id;
GLuint Render::grid_program_id;
GLuint Render::gridbuffer;
bool Render::mouse_captured = false;
int Render::gridno;
bool Render::inited = false;
int Render::WINDOW_SIZE_X = 1400, Render::WINDOW_SIZE_Y = 1000;
float Render::WINDOW_SIZE_RATIO = 1.4;
std::function<void(double)> Render::scroll_callback;

double Render::GetTime(){
	return glfwGetTime();
}

bool Render::IsKeyPressed(int key){
	return glfwGetKey(window,key) == GLFW_PRESS;
}

bool Render::IsWindowClosed(){
	return (glfwWindowShouldClose(window) == 1);
}



void Render::ScrollCallback(GLFWwindow*, double, double x){
	if(scroll_callback)
		scroll_callback(x);
}

void Render::CaptureMouse(){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	mouse_captured = true;
}
void Render::FreeMouse(){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	mouse_captured = false;
}

int Render::Init(){

	// Initialise GLFW
	if( !glfwInit() ){
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( WINDOW_SIZE_X, WINDOW_SIZE_Y, "Terrain", NULL, NULL);
	pxsizex = 2.0 / WINDOW_SIZE_X;
	pxsizey = 2.0 / WINDOW_SIZE_Y;
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

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetScrollCallback(window,ScrollCallback);

	// Background
	glClearColor(50/255.0,80.0/255.0,0.7,1.0);

	// Prepare main vertex array.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	shader_program_id = LoadShaders( "TerrainVertexShader.vertexshader", "TerrainFragmentShader.fragmentshader" );
	grid_program_id = LoadShaders( "GridVertexShader.vertexshader", "GridFragmentShader.fragmentshader" );

	// Prepare uniforms of the vertex shader
	uniform_camera_transform = glGetUniformLocation(shader_program_id, "camera_transform");
	uniform_perspective_transform = glGetUniformLocation(shader_program_id, "perspective_transform");
	uniform_xscale = glGetUniformLocation(shader_program_id, "xscale");
	uniform_sphere = glGetUniformLocation(shader_program_id, "sphere");
	uniform_camera_transform_g = glGetUniformLocation(grid_program_id, "camera_transform");
	uniform_perspective_transform_g = glGetUniformLocation(grid_program_id, "perspective_transform");
	uniform_xscale_g = glGetUniformLocation(grid_program_id, "xscale");
	uniform_sphere_g = glGetUniformLocation(grid_program_id, "sphere");
	uniform_pos = glGetUniformLocation(shader_program_id, "pos");
	uniform_light_intensity = glGetUniformLocation(shader_program_id, "light_intensity");
	uniform_light_angle = glGetUniformLocation(shader_program_id, "light_angle");
	uniform_terrainscale = glGetUniformLocation(shader_program_id, "terrainscale");
	if(uniform_camera_transform == -1 || uniform_perspective_transform == -1 || uniform_camera_transform_g == -1
	|| uniform_perspective_transform_g == -1 || uniform_pos == -1 || uniform_xscale == -1 || uniform_xscale_g == -1
	|| uniform_light_intensity == -1 || uniform_light_angle == -1 || uniform_sphere == -1 || uniform_sphere_g == -1 || uniform_terrainscale == -1){
		std::cerr << "A uniform is missing from the shader." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Prepare for rendering fonts.
	int res = init_font();
	if(res < 0) return res;

	std::vector<float> linedata;
	// horiz
	for(int y = -61; y <=61; y++)
		for(int x = -180; x < 180; x++){
			linedata.push_back(x);
			linedata.push_back(y);
			linedata.push_back(x+1);
			linedata.push_back(y);
		}
	// vert
	for(int x = -180; x <= 180; x++)
		for(int y = -61; y <61; y++){
			linedata.push_back(x);
			linedata.push_back(y);
			linedata.push_back(x);
			linedata.push_back(y+1);
		}
	glGenBuffers(1,&gridbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gridbuffer);
	glBufferData(GL_ARRAY_BUFFER, linedata.size() * sizeof(float), linedata.data(), GL_STATIC_DRAW);
	gridno = linedata.size();

	inited = true;
	return 0;
}

glm::vec2 Render::ProbeMousePos(){
	if(!mouse_captured) return glm::vec2(0,0);
	double x,y;
	glfwGetCursorPos(window,&x,&y);
	glfwSetCursorPos(window,500.0,500.0);
	return glm::vec2((x-500)/500,(y-500)/500);
}
bool Render::IsMouseDown(){
	return glfwGetMouseButton(window,0)==GLFW_PRESS;
}


void Render::FrameStart(float light_intensity, float light_angle, float xscale, bool sphere, float terrainscale){
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// Prepare camera
	glm::mat4 perspective;
	glm::mat4 cameraview;
	if(Viewpoint::active_viewpoint){
		cameraview =  glm::lookAt(glm::vec3(0.0) , 1.0f* Viewpoint::active_viewpoint->GetDirection(), glm::vec3(0.0,0.0,1.0)) * glm::inverse(Viewpoint::active_viewpoint->GetTransform());
		if(Viewpoint::active_viewpoint->ortho){
			float r = Viewpoint::active_viewpoint->ortho_range;
			perspective = glm::ortho(-r*WINDOW_SIZE_RATIO,r*WINDOW_SIZE_RATIO,-r,r,0.1f,20.0f);
 		}else{
			perspective = glm::perspective(Viewpoint::active_viewpoint->GetFOV(), WINDOW_SIZE_RATIO, 0.0005f, 4.0f);
		}
	}

	glUseProgram(grid_program_id);
	glUniformMatrix4fv(uniform_camera_transform_g, 1, GL_FALSE, &cameraview[0][0]);
	glUniformMatrix4fv(uniform_perspective_transform_g, 1, GL_FALSE, &perspective[0][0]);
   	glUniform1f(Render::uniform_xscale_g, xscale);
   	glUniform1i(Render::uniform_sphere_g, (int)sphere);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, gridbuffer);
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	glDrawArrays(GL_LINES, 0, 2*gridno);

	glDisableVertexAttribArray(0);

	glUseProgram(shader_program_id);
	glUniformMatrix4fv(uniform_camera_transform  , 1, GL_FALSE, &cameraview[0][0]);
	glUniformMatrix4fv(uniform_perspective_transform  , 1, GL_FALSE, &perspective[0][0]);
   	glUniform1f(Render::uniform_light_intensity, light_intensity);
	glUniform1f(Render::uniform_light_angle, light_angle);
	glUniform1f(Render::uniform_xscale, xscale);
	glUniform1f(Render::uniform_terrainscale, terrainscale);
	glUniform1i(Render::uniform_sphere, (int)sphere);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

void Render::FrameEnd(){
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glDisable(GL_DEPTH_TEST); // Force text to always stay on top
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
