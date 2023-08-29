/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_font
#define header_re_font

#include "precomp.h"
#include "re_resource.h"
#include "re_resourcemanager.h"
#include "re_gamecontext.h"

class RE_Font : public RE_Resource
{
public:
	enum textquality {solid, shaded, blended};

public:
	RE_Font(RE_GameContext *pgc, std::string xml_path, RE_ResourceManager *resource);
	RE_Font(std::string file, int size);
	~RE_Font();
	SDL_Texture* set_text(std::string text);
	void draw(int x, int y);
	void draw_text(int x, int y, std::string text, int s_width=0, int s_height=0);
	void set_fg_color(char r, char g, char b, char a);
	void set_bg_color(char r, char g, char b, char a);
	void resize(int new_size);
	int get_font_size(){return ptsize;}
	int get_text_height();
	int get_text_width();
private:
	TTF_Font* font;
	SDL_Texture *text_image;

	void loadfont();

	std::string font_file;
	int ptsize;
	int width;
	int height;
	SDL_Color fgColor;
	SDL_Color bgColor;
	textquality quality;
};


#endif
