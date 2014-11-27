#define GLM_FORCE_RADIANS
#include "Render.hpp"
#include "Shader.hpp"
#include "Fonts.hpp"
#include "ModelBase.hpp"
#include "Drawable.hpp"
#include "Text.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLFWwindow* window;

// All the static members of Render class.
GLint Render::uniform_model_transform;
GLint Render::uniform_anim_mode, Render::uniform_anim_phase;
GLuint Render::VertexArrayID;
float Render::pxsizex, Render::pxsizey;
GLuint Render::shader_program_id;
bool Render::inited = false;
std::shared_ptr<Node> Render::root;

double Render::GetTime(){
	return glfwGetTime();
}

bool Render::IsKeyPressed(int key){
	return glfwGetKey(window,key) == GLFW_PRESS;
}

bool Render::IsWindowClosed(){
	return (glfwWindowShouldClose(window) == 1);
}

void Render::SetRootNode(std::shared_ptr<Node> r){
	root = r;
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
	window = glfwCreateWindow( 1000, 1000, "Arcanoid", NULL, NULL);
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

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Prepare main vertex array.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	shader_program_id = LoadShaders( "VertexShaderA.vertexshader", "FragmentShaderA.fragmentshader" );

	// Prepare uniforms of the vertex shader
	uniform_model_transform = glGetUniformLocation(shader_program_id, "model_transform");
	uniform_anim_mode = glGetUniformLocation(shader_program_id,"anim_mode");
	uniform_anim_phase = glGetUniformLocation(shader_program_id,"anim_phase");
	if(uniform_model_transform == -1 || uniform_anim_mode == -1 || uniform_anim_phase == -1){
		std::cerr << "A uniform is missing from the shader." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Enable blending for several alpha effects.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void Render::RecursivellyProcessNode(std::shared_ptr<Node> n, glm::vec3 current_tranlation, glm::quat current_rotation, float current_scale){
	if(!n) return; // ignore null nodes
	if(!n->GetActive()) return;
	glm::vec3 new_translation = current_tranlation + glm::rotate(current_rotation, n->GetPosition());
	float new_scale = current_scale * n->GetScale();
	glm::quat new_rotation = current_rotation * n->GetRotation();

	glm::mat4 sc = glm::scale(glm::mat4(1.0),glm::vec3(new_scale));
	glm::mat4 tr = glm::translate(glm::mat4(1.0),new_translation);
	glm::mat4 ro = glm::toMat4(new_rotation);
	glm::mat4 transform = tr* ro * sc;

	std::shared_ptr<Drawable> d = std::dynamic_pointer_cast<Drawable>(n);
	if(d){ // skip, if this node is not drawable
		std::shared_ptr<Model> m = ModelBase::GetInstance().GetModel(d->model_id);
		if(m){
			glUniformMatrix4fv(uniform_model_transform, 1, GL_FALSE, &transform[0][0]);
			glUniform1i(uniform_anim_mode, d->anim_mode);
			if(d->anim_mode != 0){
				float anim_phase = (GetTime() - d->anim_start)/d->anim_length;
				glUniform1f(uniform_anim_phase, anim_phase);
				if(anim_phase >= 1.0) d->on_animation_finished.Happen(d->anim_mode);
			}
			m->metaDraw(d->variant);
		}else{
			std::cerr << "Warning: A drawable has no model in base (" << d->model_id << ")" << std::endl;
		}
	}
	for(auto ch : n->children) RecursivellyProcessNode(ch, new_translation, new_rotation, new_scale);
}

void Render::Frame(){
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the board shader
	glUseProgram(shader_program_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//std::cout << "starting render" << std::endl;
	RecursivellyProcessNode(root, glm::vec3(0.0), glm::quat(), 1.0);

/* Temporarily disabled
	for(auto t : Text::texts){
		if(!t->GetActiveAbsolute()) continue;

		glm::vec2 pos = t->GetPos();
		glm::vec2 off = t->px_offset;
		glm::vec3 color = t->color;
		float scale = t->GetScale();

		render_text(t->text.c_str(), pos.x + off.x*pxsizex, pos.y - off.y*pxsizey, color.r, color.g, color.b, t->size*scale, pxsizex, pxsizey);
	}
*/

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

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
