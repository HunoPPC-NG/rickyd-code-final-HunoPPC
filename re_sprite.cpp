/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "re_sprite.h"
#include "tinyxml2.h"

//constructor
RE_Sprite::RE_Sprite(RE_GameContext* pgc, std::string xml_path, RE_ResourceManager *resource)
	: RE_Resource(pgc, NULL, resource)
{
	if(!resource->file_exists(xml_path))
	{
		std::cerr << "RE_Sprite: couldn't load sprite (" << xml_path <<"): exiting...\n";
		exit(-1);
	}

	tinyxml2::XMLElement* sprite_elem = resource->getElement(xml_path);
	std::string file = sprite_elem->Attribute( "file" );
	file = resource->get_data_dir() + file;
	//std::cout << "RE_Sprite: loading " << file << "\n";
	int width  = sprite_elem->FirstChildElement( "grid" )->IntAttribute("width");
	int height = sprite_elem->FirstChildElement( "grid" )->IntAttribute("height");
	int col    = sprite_elem->FirstChildElement( "grid" )->IntAttribute("cols");
	int row    = sprite_elem->FirstChildElement( "grid" )->IntAttribute("rows");

	load(file, width, height, col, row);
}

RE_Sprite::RE_Sprite(std::string file, int width, int height, int col, int row)
{
	load(file, width, height, col, row);
}

//destructor
RE_Sprite::~RE_Sprite()
{
	SDL_DestroyTexture ( sprite_image );
}

//public
void RE_Sprite::set_frame(int index)
{
	if(index > num_frames - 1)
	{
		//cerr << "RE_Sprite: invalid frame index ( " << index << " > " << num_frames - 1 << ")\n";
		index = 0; //reset
	}

	frame_index = index;
}

void RE_Sprite::draw(int x, int y)
{
	int frow = floor(frame_index/num_columns);
	int fcol = frame_index - (frow*num_columns);

	//cout << "RE_Sprite: index("<< frame_index<<") frow("<<frow<<") fcol("<<fcol<<")\n";
	SDL_Rect draw_rect;
	draw_rect.x = fcol * sprite_width;
	draw_rect.y = frow * sprite_height;
	draw_rect.w = sprite_width;
	draw_rect.h = sprite_height;

	//SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "0");
	gc->draw(sprite_image, &draw_rect, x, y);
	//SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "1");
}

//private
void RE_Sprite::load(std::string file, int width, int height, int col, int row)
{
	frame_index = 0;

	filename = file;

	sprite_image = NULL;

	if(gc->renderer == NULL)
		std::cerr << "RE_Sprite: Invalid renderer\n";
	else
		sprite_image = IMG_LoadTexture( gc->renderer, filename.c_str());

	if(sprite_image == NULL)
	{
		cerr << "RE_Sprite: couldn't load " << filename << " error:"<< IMG_GetError() << endl;
		exit(1);
	}

	SDL_SetTextureAlphaMod(sprite_image, 255);
	SDL_SetTextureBlendMode(sprite_image, SDL_BLENDMODE_BLEND);

	Uint32 format = 0;
	int access = 0;

	SDL_QueryTexture(sprite_image, &format, &access, &image_width, &image_height);

	sprite_width = width;
	sprite_height = height;

	int tcol = image_width  / sprite_width;
	int trow = image_height / sprite_height;

	if(col > 0 && col <= tcol)
	{
		num_columns = col;
	}
	else
	{
		cerr << "RE_Sprite: invalid column number ( " << col << ")\n";
		cerr << "           detected " << tcol << "columns for"<< filename <<"\n";
		exit(1);
	}

	if(row > 0 && row <= trow)
	{
		num_rows = row;
	}
	else
	{
		cerr << "RE_Sprite: invalid row number ( " << row << ")\n";
		cerr << "           detected " << trow << "rows for"<< filename <<"\n";
		exit(1);
	}

	num_frames = num_rows * num_columns;

	//cout << "RE_Sprite: num_frames("<< num_frames <<") num_rows("<<num_rows<<") num_columns("<<num_columns<<")\n";
	//cout << "           image_width("<<image_width<<") image_height("<<image_height<<") sprite_width("<<sprite_width<<") sprite_height("<<sprite_height<<")\n";
}
