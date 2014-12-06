#define GLM_FORCE_RADIANS
#include "Render.hpp"
#include "Shader.hpp"
#include "Fonts.hpp"
#include "ModelBase.hpp"
#include "Drawable.hpp"
#include "Text.hpp"
#include "Viewpoint.hpp"
#include "Light.hpp"
#include "World.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLFWwindow* window;

// All the static members of Render class.
GLint Render::uniform_model_transform, Render::uniform_camera_transform, Render::uniform_perspective_transform;
GLint Render::uniform_lightpos, Render::uniform_diffuse, Render::uniform_spatial, Render::uniform_ambient;
GLint Render::uniform_anim_mode, Render::uniform_anim_phase;
GLint Render::uniform_single_color, Render::uniform_boring_color;
GLuint Render::VertexArrayID;
float Render::pxsizex, Render::pxsizey;
GLuint Render::shader_program_id;
bool Render::inited = false;
std::shared_ptr<Node> Render::root;
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

void Render::SetRootNode(std::shared_ptr<Node> r){
	root = r;
}

void Render::ScrollCallback(GLFWwindow*, double, double x){
	if(scroll_callback)
		scroll_callback(x);
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
	window = glfwCreateWindow( 1000, 1000, "Bubbles", NULL, NULL);
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
	glfwSetScrollCallback(window,ScrollCallback);

	// Black background
	glClearColor(100/255.0,200.0/255.0,1.0,1.0);

	// Prepare main vertex array.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	shader_program_id = LoadShaders( "VertexShaderA.vertexshader", "FragmentShaderA.fragmentshader" );

	// Prepare uniforms of the vertex shader
	uniform_model_transform = glGetUniformLocation(shader_program_id, "model_transform");
	uniform_camera_transform = glGetUniformLocation(shader_program_id, "camera_transform");
	uniform_perspective_transform = glGetUniformLocation(shader_program_id, "perspective_transform");
	uniform_lightpos = glGetUniformLocation(shader_program_id, "lightpos_global");
	uniform_anim_mode = glGetUniformLocation(shader_program_id,"anim_mode");
	uniform_anim_phase = glGetUniformLocation(shader_program_id,"anim_phase");
	uniform_diffuse = glGetUniformLocation(shader_program_id,"material_diffuse_ratio");
	uniform_spatial = glGetUniformLocation(shader_program_id,"material_spatial_ratio");
	uniform_ambient = glGetUniformLocation(shader_program_id,"material_ambient_ratio");
	uniform_single_color = glGetUniformLocation(shader_program_id,"single_color");
	uniform_boring_color = glGetUniformLocation(shader_program_id,"boring_color");
	if(uniform_model_transform == -1 || uniform_anim_mode == -1 || uniform_anim_phase == -1 || uniform_camera_transform == -1 || uniform_perspective_transform == -1 ||
       uniform_lightpos == -1 || uniform_diffuse == -1 || uniform_spatial == -1 || uniform_ambient == -1 || uniform_single_color == -1 || uniform_boring_color == -1){
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

void Render::RecursivellyProcessNode(std::shared_ptr<Node> n, glm::mat4 current_transform){
	if(!n) return; // ignore null nodes
	if(!n->GetActive()) return;

	glm::mat4 transform = current_transform * n->GetTransform();

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
			if(d->culling == 0){
				glDisable(GL_CULL_FACE);
			}else if(d->culling == 1){
				glFrontFace(GL_CCW);
				glEnable(GL_CULL_FACE);
			}else if(d->culling == 2){
				glFrontFace(GL_CW);
				glEnable(GL_CULL_FACE);
			}
			glUniform1f(uniform_diffuse, d->diffuse);
			glUniform1f(uniform_spatial, d->spatial);
			glUniform1f(uniform_ambient, d->ambient);
			{ // RENDERING A MODEL
				unsigned int variant = d->variant %= m->variants;

				if(!m->single_color){
					// Select buffers to use
					glBindBuffer(GL_ARRAY_BUFFER, m->buffer_vertex);
					glVertexAttribPointer( 0, MODEL_DIMEN, GL_FLOAT, GL_FALSE, 0, (void*)0 );
					glBindBuffer(GL_ARRAY_BUFFER, m->buffer_normals);
					glVertexAttribPointer( 2, MODEL_DIMEN, GL_FLOAT, GL_FALSE, 0, (void*)0 );
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, m->buffers_color[variant]);
					glVertexAttribPointer( 1, MODEL_COLOR_SIZE, GL_FLOAT, GL_FALSE, 0, (void*)0 );
					glUniform1i(uniform_single_color,0);
				}else{
					// Select buffers to use
					glBindBuffer(GL_ARRAY_BUFFER, m->buffer_vertex);
					glVertexAttribPointer( 0, MODEL_DIMEN, GL_FLOAT, GL_FALSE, 0, (void*)0 );
					glBindBuffer(GL_ARRAY_BUFFER, m->buffer_normals);
					glVertexAttribPointer( 2, MODEL_DIMEN, GL_FLOAT, GL_FALSE, 0, (void*)0 );
					glDisableVertexAttribArray(1);
					glUniform1i(uniform_single_color,1);
					glUniform4fv(uniform_boring_color, 1, glm::value_ptr(m->boring_colors[variant]));

				}
				// Draw the model
				glDrawArrays(m->mode, 0, m->ppp*m->size);
			}
		}else{
			std::cerr << "Warning: A drawable has no model in base (" << d->model_id << ")" << std::endl;
		}
	}
	for(auto ch : n->children) RecursivellyProcessNode(ch, transform);
}

void Render::Frame(){
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the board shader
	glUseProgram(shader_program_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Prepare main light
	if(Light::lights.size() > 0){
		Light* l = Light::lights[0];
		glm::vec3 lightpos(l->GetGlobalTransform()[3]);
		glUniform3f(uniform_lightpos,lightpos.x,lightpos.y,lightpos.z);
	}
	// Prepare camera
	if(Viewpoint::active_viewpoint){
		glm::mat4 cameraview =  glm::lookAt(glm::vec3(0.0) , 1.0f* Viewpoint::active_viewpoint->GetDirection(), World::up) * glm::inverse(Viewpoint::active_viewpoint->GetGlobalTransform());
		glm::mat4 perspective = glm::perspective(Viewpoint::active_viewpoint->GetFOV(), 1.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(uniform_camera_transform, 1, GL_FALSE, &cameraview[0][0]);
		glUniformMatrix4fv(uniform_perspective_transform, 1, GL_FALSE, &perspective[0][0]);
	}

	RecursivellyProcessNode(root, glm::mat4(1.0));

	for(auto t : Text::texts){
		if(!t->active) continue;
		glm::vec2 off = t->px_offset;
		glm::vec3 color = t->color;

		render_text(t->text.c_str(), -1.0 + off.x*pxsizex, 1.0 - off.y*pxsizey, color.r, color.g, color.b, t->size, pxsizex, pxsizey);
	}

	glDisableVertexAttribArray(2);
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
