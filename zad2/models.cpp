// This includable file provides vertex coordinates and colors for all models,
// mainly card templates and highlight shape.

#define CARD_MODELS 4

// How many vertices does each card model have?
GLfloat card_sizes[CARD_MODELS+1] = {12,6,6,6,6};

GLfloat card_vertices[CARD_MODELS+1][24] = {
	{
		-1.0f, -1.0f, 0.0f, 0.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, -1.0f,  1.0f,
		1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  1.0f,
		1.0f,  1.0f, 0.0f, 0.0f,  1.0f, -1.0f,
	},{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f,
	},{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f,
	},{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f,
	},{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f,
	}
};

#define COLOR_D_BROWN 85/255.0f, 46/255.0f, 1/255.0f
#define COLOR_L_BROWN 150/255.0f, 81/255.0f, 2/255.0f

GLfloat card_colors[CARD_MODELS+1][36] = {
	{
		COLOR_D_BROWN, COLOR_D_BROWN, COLOR_L_BROWN,
		COLOR_D_BROWN, COLOR_L_BROWN, COLOR_L_BROWN,
		COLOR_D_BROWN, COLOR_D_BROWN, COLOR_L_BROWN,
		COLOR_D_BROWN, COLOR_L_BROWN, COLOR_L_BROWN,
	},{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.2f, 0.7f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.2f, 0.7f,
	},{
		0.0f, 0.5f, 0.0f,
		0.0f, 0.2f, 0.2f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.2f, 0.2f,
	},{
		0.9f, 0.6f, 0.0f,
		1.0f, 1.0f, 0.2f,
		0.9f, 0.6f, 0.0f,
		0.9f, 0.6f, 0.0f,
		0.9f, 0.6f, 0.0f,
		1.0f, 1.0f, 0.2f,
	},{
		0.0f, 1.0f, 0.8f,
		0.0f, 0.8f, 0.9f,
		0.0f, 1.0f, 0.8f,
		0.0f, 1.0f, 0.8f,
		0.0f, 1.0f, 0.8f,
		0.0f, 0.8f, 0.9f,
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
