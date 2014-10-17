#ifndef FONTS_HPP
#define FONTS_HPP

// Must be called before any text is rendered, but OpenGL should be
// initialized first. Returns 0 on success.
int init_font();
void render_text(const char *text, float x, float y, float colorr, float colorg, float colorb, int fontsize = 48, float sx = 1.0, float sy = 1.0);


#endif //FONTS_HPP
