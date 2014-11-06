#include <glm/glm.hpp>
#include "ModelBase.h"
#include "Brick.h"
#include "Render.h"

int main(){
	// Prepare the renderer.
	int n = Render::Init();

	if(n) return n;
	glm::vec4 test;

	std::shared_ptr<BrickBody> bb = std::make_shared<BrickBody>(glm::vec2(-0.2,-0.2));
	std::shared_ptr<BrickDrawable> bd = std::make_shared<BrickDrawable>(glm::vec2(0.4,0.4));
	bb->LinkChild(bd);

	// This is the main loop.
	do{
		// Draw a single frame.
		Render::Frame();

		// If the ESC key was pressed or the window was closed, break the loop
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );
}
