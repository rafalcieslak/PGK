// This includable file provides vertex coordinates and colors for all models,
// mainly card templates and highlight shape.

#include "models.hpp"

// The 0th model represents card cover.

// How many triangles does each card model have?
GLfloat card_sizes[CARD_MODELS+1] = {4,3,4,4,6,5,6,4,4};

GLfloat card_vertices[CARD_MODELS+2][48] = {
	{   // card cover
		-1.0f, -1.0f, 0.0f, 0.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, -1.0f,  1.0f,
		 1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  1.0f,
		 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, -1.0f,
	},{ // triangle
		-1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -0.5f,  0.0f, 0.5f,
	},{ // rectangle
		-1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,
	},{ // fat line
		-1.0f, -1.0f,   1.0f,  -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,   1.0f,   1.0f, -1.0f,  1.0f,
		-0.5f,  0.75f,  0.75f, -0.5f,  0.5f, -0.75f,
		-0.5f,  0.75f, -0.75f,  0.5f,  0.5f, -0.75f,
	},{ // cross
		-1.0f,  -1.0f,   1.0f,  -1.0f,  1.0f,  1.0f,
		-1.0f,  -1.0f,   1.0f,   1.0f, -1.0f,  1.0f,
		-0.5f,  -0.25f,  0.5f,  -0.25f, 0.5f,  0.25f,
		-0.5f,  -0.25f, -0.5f,   0.25f, 0.5f,  0.25f,
		-0.25f, -0.5f,   0.25f, -0.5f,  0.25f, 0.5f,
		-0.25f, -0.5f,  -0.25f,  0.5f,  0.25f, 0.5f,
	},{ // five-pointed star
		-1.0f,   -1.0f,   1.0f,   -1.0f,    1.0f,    1.0f,
		-1.0f,   -1.0f,   1.0f,    1.0f,   -1.0f,    1.0f,
		 0.362f, -0.498f, 0.132f,  0.190f, -0.223f, -0.072f,
		 0.585f,  0.190f, 0.0f,   -0.235f, -0.585f,  0.190f,
		 0.0f,    0.615f, 0.223f, -0.072f, -0.362f, -0.498f,
	},{ // hexagon
		-1.0f,   -1.0f,   1.0f,   -1.0f,    1.0f,   1.0f,
		-1.0f,   -1.0f,   1.0f,    1.0f,   -1.0f,   1.0f,
		-0.283f,  0.490f, 0.283f,  0.490f, -0.566f, 0.0f,
		 0.283f,  0.490f, 0.566f,  0.0f,   -0.566f, 0.0f,
		-0.283f, -0.490f, 0.283f, -0.490f, -0.566f, 0.0f,
		 0.283f, -0.490f, 0.566f,  0.0f,   -0.566f, 0.0f,
	},{ // rhombus
		-1.0f,  -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,
		-1.0f,  -1.0f,  1.0f,   1.0f, -1.0f,  1.0f,
		 0.75f, -0.5f, -0.25f, -0.5f,  0.25f, 0.5f,
		-0.25f, -0.5f,  0.25f,  0.5f, -0.75f, 0.5f,
	},{ // two triangles
		-1.0f,  -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f,  -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,
		-0.25f, -0.5f, -0.25f, 0.5f, -0.75f, 0.0f,
		 0.25f, -0.5f,  0.25f, 0.5f,  0.75f, 0.0f,
	}
};

#define RGB(r,g,b) r/255.0f, g/255.0f, b/255.0f

#define COLOR_D_BROWN  RGB(85, 46, 1)
#define COLOR_L_BROWN  RGB(150, 81, 2)
#define COLOR_M_BLUE   RGB(8, 117, 241)
#define COLOR_D_BLUE   RGB(25, 77, 137)
#define COLOR_H_BLUE   RGB(5, 36, 71)
#define COLOR_M_GREEN  RGB(35, 184, 82)
#define COLOR_D_GREEN  RGB(38, 94, 56)
#define COLOR_H_GREEN  RGB(0, 66, 20)
#define COLOR_M_YELLOW RGB(234, 234, 14)
#define COLOR_D_YELLOW RGB(209, 209, 89)
#define COLOR_H_YELLOW RGB(168, 168, 0)
#define COLOR_M_RED    RGB(226, 65, 65)
#define COLOR_D_RED    RGB(96, 25, 25)
#define COLOR_H_RED    RGB(178, 0, 0)
#define COLOR_M_VIOLET RGB(146, 42, 178)
#define COLOR_D_VIOLET RGB(98, 47, 114)
#define COLOR_H_VIOLET RGB(76, 0, 99)
#define COLOR_M_GRAY   RGB(165, 165, 165)
#define COLOR_D_GRAY   RGB(117, 117, 117)
#define COLOR_H_GRAY   RGB(66, 66, 66)
#define COLOR_M_PINK   RGB(209, 43, 148)
#define COLOR_D_PINK   RGB(117, 49, 92)
#define COLOR_H_PINK   RGB(153, 0, 96)
#define COLOR_M_CYAN   RGB(58, 175, 160)
#define COLOR_D_CYAN   RGB(42, 94, 87)
#define COLOR_H_CYAN   RGB(0, 91, 79)

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
	},{
		COLOR_M_VIOLET, COLOR_M_VIOLET, COLOR_D_VIOLET,
		COLOR_M_VIOLET, COLOR_M_VIOLET, COLOR_D_VIOLET,
		COLOR_H_VIOLET, COLOR_H_VIOLET, COLOR_H_VIOLET,
		COLOR_H_VIOLET, COLOR_H_VIOLET, COLOR_H_VIOLET,
		COLOR_H_VIOLET, COLOR_H_VIOLET, COLOR_H_VIOLET,
	},{
		COLOR_M_GRAY, COLOR_M_GRAY, COLOR_D_GRAY,
		COLOR_M_GRAY, COLOR_M_GRAY, COLOR_D_GRAY,
		COLOR_H_GRAY, COLOR_H_GRAY, COLOR_H_GRAY,
		COLOR_H_GRAY, COLOR_H_GRAY, COLOR_H_GRAY,
		COLOR_H_GRAY, COLOR_H_GRAY, COLOR_H_GRAY,
		COLOR_H_GRAY, COLOR_H_GRAY, COLOR_H_GRAY,
	},{
		COLOR_M_PINK, COLOR_M_PINK, COLOR_D_PINK,
		COLOR_M_PINK, COLOR_M_PINK, COLOR_D_PINK,
		COLOR_H_PINK, COLOR_H_PINK, COLOR_H_PINK,
		COLOR_H_PINK, COLOR_H_PINK, COLOR_H_PINK,
	},{
		COLOR_M_CYAN, COLOR_M_CYAN, COLOR_D_CYAN,
		COLOR_M_CYAN, COLOR_M_CYAN, COLOR_D_CYAN,
		COLOR_H_CYAN, COLOR_H_CYAN, COLOR_H_CYAN,
		COLOR_H_CYAN, COLOR_H_CYAN, COLOR_H_CYAN,
	}
};

GLfloat highlight_vertices[] = {
	-1.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f,
	 1.0f, -1.0f,  1.0f, -0.5f,  0.5f, -1.0f,
	-1.0f,  1.0f, -1.0f,  0.5f, -0.5f,  1.0f,
	 1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  1.0f,
};
GLfloat highlight_colors[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
};
