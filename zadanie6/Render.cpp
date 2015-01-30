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
#include "TextureLoader.hpp"

GLFWwindow* window;

GLint Render::uniform_camera_transform, Render::uniform_perspective_transform;
GLint Render::uniform_color_diffuse, Render::uniform_color_spectral, Render::uniform_color_ambient;
GLint Render::uniform_tex_spec, Render::uniform_tex_amb, Render::uniform_tex_diff;
GLint Render::uniform_use_tex_spec, Render::uniform_use_tex_amb, Render::uniform_use_tex_diff;
GLint Render::uniform_specular_hardness, Render::uniform_lightpos, Render::uniform_alpha;
GLint Render::uniform_contour_scale;
GLuint Render::VertexArrayID;
float Render::pxsizex, Render::pxsizey;
GLuint Render::current_shader, Render::basic_shader, Render::toon_shader, Render::toon2_shader, Render::toon3_shader;
bool Render::inited = false;
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

void Render::SetShaderMode(ShaderMode s){
	switch(s){
		case SHADER_MODE_BASIC:
			current_shader = basic_shader;
			break;
		case SHADER_MODE_TOON:
			current_shader = toon_shader;
			break;
		case SHADER_MODE_TOON2:
			current_shader = toon2_shader;
			break;
		case SHADER_MODE_TOON3:
			current_shader = toon3_shader;
			break;
	}
	// Prepare uniforms of the vertex shader
	uniform_camera_transform = glGetUniformLocation(current_shader, "camera_transform");
	uniform_perspective_transform = glGetUniformLocation(current_shader, "perspective_transform");
	uniform_color_ambient  = glGetUniformLocation(current_shader, "color_ambient");
	uniform_color_diffuse  = glGetUniformLocation(current_shader, "color_diffuse");
	uniform_color_spectral = glGetUniformLocation(current_shader, "color_spectral");
	uniform_tex_amb  = glGetUniformLocation(current_shader, "tex_sampler_ambient");
	uniform_tex_spec  = glGetUniformLocation(current_shader, "tex_sampler_spectral");
	uniform_tex_diff  = glGetUniformLocation(current_shader, "tex_sampler_diffuse");
	uniform_use_tex_amb  = glGetUniformLocation(current_shader, "use_ambient_texture");
	uniform_use_tex_spec  = glGetUniformLocation(current_shader, "use_spectral_texture");
	uniform_use_tex_diff  = glGetUniformLocation(current_shader, "use_diffuse_texture");
	uniform_specular_hardness  = glGetUniformLocation(current_shader, "specular_hardness");
	uniform_lightpos  = glGetUniformLocation(current_shader, "lightpos_global");
	uniform_alpha  = glGetUniformLocation(current_shader, "alpha");
	uniform_contour_scale  = glGetUniformLocation(current_shader, "contour_scale");
	if(uniform_camera_transform == -1 || uniform_perspective_transform == -1){
		std::cerr << "An essential uniform is missing from the shader." << std::endl;
		glfwTerminate();
		exit(1);
	}
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
	window = glfwCreateWindow( 1000, 1000, "Model viewer", NULL, NULL);
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window,ScrollCallback);

	// Background
	glClearColor(0.2,0.2,0.2,1.0);

	// Prepare main vertex array.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	basic_shader = LoadShaders( "BasicVertexShader.vertexshader", "BasicFragmentShader.fragmentshader" );
	toon_shader = LoadShaders( "ToonVertexShader.vertexshader", "ToonFragmentShader.fragmentshader" );
	toon2_shader = LoadShaders( "Toon2VertexShader.vertexshader", "Toon2GeometryShader.geometryshader", "Toon2FragmentShader.fragmentshader" );
	toon3_shader = LoadShaders( "Toon3VertexShader.vertexshader", "Toon3GeometryShader.geometryshader", "Toon3FragmentShader.fragmentshader" );

	SetShaderMode(SHADER_MODE_BASIC);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	glEnable(GL_CULL_FACE);

	// Prepare for rendering fonts.
	int res = init_font();
	if(res < 0) return res;

	inited = true;
	return 0;
}

glm::vec2 Render::GetMousePos(){
	double x,y;
	glfwGetCursorPos(window,&x,&y);
	//glfwSetCursorPos(window,500.0,500.0);
	return glm::vec2((x-500)/500,(y-500)/500);
}
bool Render::IsMouseDown(int button){
	return glfwGetMouseButton(window,button)==GLFW_PRESS;
}


void Render::Frame(const std::vector<std::shared_ptr<Mesh>> &meshes, glm::vec3 lightpos, float near, float far, float contour_scale){

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(current_shader);

	// Prepare camera
	glm::mat4 perspective;
	glm::mat4 cameraview;
	if(Viewpoint::active_viewpoint){
		cameraview =  glm::lookAt(glm::vec3(0.0) , 1.0f* Viewpoint::active_viewpoint->GetDirection(), glm::vec3(0.0,1.0,0.0)) * glm::inverse(Viewpoint::active_viewpoint->GetTransform());
		perspective = glm::perspective(Viewpoint::active_viewpoint->GetFOV(), 1.0f, near, far);
	}
	glUniformMatrix4fv(uniform_camera_transform  , 1, GL_FALSE, &cameraview[0][0]);
	glUniformMatrix4fv(uniform_perspective_transform  , 1, GL_FALSE, &perspective[0][0]);

	glUniform1i(uniform_tex_amb, 0);
	glUniform1i(uniform_tex_diff, 1);
	glUniform1i(uniform_tex_spec, 2);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glUniform1f(uniform_contour_scale, contour_scale);

	int id, use_tex;
	for(auto m : meshes){
		if(m->hidden) continue;

		glUniform3fv(uniform_color_ambient, 1, glm::value_ptr(m->material->ambient));
		glUniform3fv(uniform_color_diffuse, 1, glm::value_ptr(m->material->diffuse));
		glUniform3fv(uniform_color_spectral, 1, glm::value_ptr(m->material->spectral));

		glUniform3fv(uniform_lightpos, 1, glm::value_ptr(lightpos));

		if(m->material->ambient_tex_path != "") id = GetTexture(m->material->base_dir, m->material->ambient_tex_path);
		use_tex = (m->material->ambient_tex_path != "" && id != GL_INVALID_VALUE);
		glUniform1i(uniform_use_tex_amb,use_tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
		if(m->material->diffuse_tex_path != "") id = GetTexture(m->material->base_dir, m->material->diffuse_tex_path);
		use_tex = (m->material->diffuse_tex_path != "" && id != GL_INVALID_VALUE);
		glUniform1i(uniform_use_tex_diff,use_tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, id);
		if(m->material->spectral_tex_path != "") id = GetTexture(m->material->base_dir, m->material->spectral_tex_path);
		use_tex = (m->material->spectral_tex_path != "" && id != GL_INVALID_VALUE);
		glUniform1i(uniform_use_tex_spec,use_tex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, id);

		glUniform1f(uniform_specular_hardness, m->material->spectral_exponent);
		glUniform1f(uniform_alpha, m->material->alpha);

		m->Render();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

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
	glDeleteProgram(basic_shader);
	glDeleteProgram(toon_shader);
	glDeleteProgram(toon2_shader);
	glDeleteProgram(toon3_shader);

	// Close the window.
	glfwTerminate();
}
