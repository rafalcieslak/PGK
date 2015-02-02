#include <GL/glew.h>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Shader.hpp"
#include "Fonts.hpp"

/*
This code is mildly inspired by
http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
*/


FT_Library ft;
FT_Face face;
GLuint text_shader_program_id;
GLuint tex, uniform_tex, uniform_color;
GLuint vbo;

int init_font(){
	if(FT_Init_FreeType(&ft)) {
		std::cerr << "Could not init freetype library" << std::endl;
		return -1;
	}

	if(FT_New_Face(ft, "Ubuntu.ttf", 0, &face)) {
		std::cerr << "Failed to open font. Are you in the right directory?" << std::endl;
		return -1;
	}

	text_shader_program_id = LoadShaders( "TextVertexShader.vertexshader", "TextFragmentShader.fragmentshader" );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	uniform_tex = glGetUniformLocation(text_shader_program_id,"tex");
	uniform_color = glGetUniformLocation(text_shader_program_id,"color");
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenBuffers(1, &vbo);

	return 0;
}

void render_text(const char *text, float x, float y, float colorr, float colorg, float colorb, int fontsize, float sx, float sy) {
	const char *p;

	glUseProgram(text_shader_program_id);
    FT_Set_Pixel_Sizes(face, 0, fontsize);

	GLfloat color[4] = {colorr, colorg, colorb, 1};
	glUniform4fv(uniform_color, 1, color);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	for(p = text; *p; p++) {
		if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		FT_GlyphSlot g = face->glyph;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
		);

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,     -y2    , 0, 0},
			{x2 + w, -y2    , 1, 0},
			{x2,     -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}
