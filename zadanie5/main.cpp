#include "Tile.hpp"
#include "Render.hpp"

int main(){
	auto t = Tile::CreateFromHGTFile("/tpm/" + Tile::TileString(50,16) + ".hgt");

	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	do{

		Render::Frame();

	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

	Render::CleanUp();
}
