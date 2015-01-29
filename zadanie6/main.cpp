#include "Render.hpp"
#include <iostream>

int main(){
	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	do{
		Render::Frame();
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

}
