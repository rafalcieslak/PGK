// This includable file provides vertex coordinates and colors for all models,
// mainly card templates and highlight shape.

#define CARD_MODELS 4

// The 0th model represents card cover.

// How many triangles does each card model have?
GLfloat card_sizes[CARD_MODELS+1] = {4,3,4,4,6};

GLfloat card_vertices[CARD_MODELS+1][48] = {
	{
		-1.0f, -1.0f, 0.0f, 0.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, -1.0f,  1.0f,
		 1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  1.0f,
		 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, -1.0f,
	},{
		-1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -0.5f,  0.0f, 0.5f,
	},{
		-1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,
	},{
		-1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
		-0.5f, 0.75f, 0.75f, -0.5f, 0.5f, -0.75f,
		-0.5f, 0.75f, -0.75f, 0.5f, 0.5f, -0.75f,
	},{
		-1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
		-0.5f, -0.25f, 0.5f, -0.25f, 0.5f, 0.25f,
		-0.5f, -0.25f, -0.5f, 0.25f, 0.5f, 0.25f,
		-0.25f, -0.5f, 0.25f, -0.5f, 0.25f, 0.5f,
		-0.25f, -0.5f, -0.25f, 0.5f, 0.25f, 0.5f,
	}
};

#define RGB(r,g,b) r/255.0f, g/255.0f, b/255.0f

#define COLOR_D_BROWN RGB(85, 46, 1)
#define COLOR_L_BROWN RGB(150, 81, 2)
#define COLOR_M_BLUE RGB(8, 117, 241)
#define COLOR_D_BLUE RGB(25, 77, 137)
#define COLOR_H_BLUE RGB(5, 36, 71)
#define COLOR_M_GREEN RGB(35, 184, 82)
#define COLOR_D_GREEN RGB(38, 94, 56)
#define COLOR_H_GREEN RGB(0, 66, 20)
#define COLOR_M_YELLOW RGB(234, 234, 14)
#define COLOR_D_YELLOW RGB(209, 209, 89)
#define COLOR_H_YELLOW RGB(168, 168, 0)
#define COLOR_M_RED RGB(226, 65, 65)
#define COLOR_D_RED RGB(96, 25, 25)
#define COLOR_H_RED RGB(178, 0, 0)

GLfloat card_colors[CARD_MODELS+1][60] = {
	{
		COLOR_D_BROWN, COLOR_D_BROWN, COLOR_L_BROWN,
		COLOR_D_BROWN, COLOR_L_BROWN, COLOR_L_BROWN,
		COLOR_D_BROWN, COLOR_D_BROWN, COLOR_L_BROWN,
		COLOR_D_BROWN, COLOR_L_BROWN, COLOR_L_BROWN,
	},{
		COLOR_M_BLUE, COLOR_M_BLUE, COLOR_D_BLUE,
		COLOR_M_BLUE, COLOR_M_BLUE, COLOR_D_BLUE,
		COLOR_H_BLUE, COLOR_H_BLUE, COLOR_H_BLUE,
	},{
		COLOR_M_GREEN, COLOR_M_GREEN, COLOR_D_GREEN,
		COLOR_M_GREEN, COLOR_M_GREEN, COLOR_D_GREEN,
		COLOR_H_GREEN, COLOR_H_GREEN, COLOR_H_GREEN,
		COLOR_H_GREEN, COLOR_H_GREEN, COLOR_H_GREEN,
	},{
		COLOR_M_YELLOW, COLOR_M_YELLOW, COLOR_D_YELLOW,
		COLOR_M_YELLOW, COLOR_M_YELLOW, COLOR_D_YELLOW,
		COLOR_H_YELLOW, COLOR_H_YELLOW, COLOR_H_YELLOW,
		COLOR_H_YELLOW, COLOR_H_YELLOW, COLOR_H_YELLOW,
	},{
		COLOR_M_RED, COLOR_M_RED, COLOR_D_RED,
		COLOR_M_RED, COLOR_M_RED, COLOR_D_RED,
		COLOR_H_RED, COLOR_H_RED, COLOR_H_RED,
		COLOR_H_RED, COLOR_H_RED, COLOR_H_RED,
		COLOR_H_RED, COLOR_H_RED, COLOR_H_RED,
		COLOR_H_RED, COLOR_H_RED, COLOR_H_RED,
	}
};

GLfloat highlight_vertices[] = {
	-1.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f,
	1.0f, -1.0f, 1.0f, -0.5f, 0.5f, -1.0f,
	-1.0f, 1.0f, -1.0f, 0.5f, -0.5f, 1.0f,
	1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 1.0f,
};
GLfloat highlight_colors[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
};
