/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_sprite
#define header_re_sprite

#include "precomp.h"
#include "re_resource.h"
#include "re_resourcemanager.h"
#include "re_gamecontext.h"

class RE_Sprite : public RE_Resource
{
public:
	RE_Sprite(RE_GameContext *pgc, std::string xml_path, RE_ResourceManager *resource);
	RE_Sprite(std::string file, int width, int height, int col=0, int row=0);
	~RE_Sprite();
	void set_frame(int index);
	void draw(int x, int y);
	int get_frame_count() {return num_frames;}
	int get_width() {return sprite_width;}
	int get_height() {return sprite_height;}

private:
	//functions
	void load(std::string file, int width, int height, int col=0, int row=0);
	//attributes
	std::string filename;
	SDL_Texture *sprite_image;
	int num_frames;
	int frame_index;
	int sprite_width;
	int sprite_height;
	int image_width;
	int image_height;
	int num_columns;
	int num_rows;
};

#endif
