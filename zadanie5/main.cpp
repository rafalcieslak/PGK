#include "Tile.hpp"
#include "Render.hpp"
#include "Text.hpp"

int main(){
	// Prepare the renderer.
	int n = Render::Init();
	if(n) return n;

	auto q = std::make_shared<Text>("AAAaaa", glm::vec2(50,50));
	Tile::Init();
	auto t = Tile::CreateFromHGTFile("/tmp/" + Tile::TileString(45,7) + ".hgt");
	t->Prepare();

	do{

		Render::FrameStart();
		t->Render();
		Render::FrameEnd();
	}while( !Render::IsKeyPressed(GLFW_KEY_ESCAPE ) && !Render::IsWindowClosed() );

	Render::CleanUp();
}
