/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "re_font.h"
#include "tinyxml2.h"

RE_Font::RE_Font(RE_GameContext *pgc, std::string xml_path, RE_ResourceManager *resource)
	: RE_Resource(pgc)
{
	tinyxml2::XMLElement* font_elem = resource->getElement(xml_path);

	font_file = font_elem->Attribute( "file" );
	font_file = resource->get_data_dir() + font_file;

	ptsize = font_elem->IntAttribute("size");
	font = NULL;
	text_image = NULL;
	width = 0;
	height= 0;
	loadfont();
}

RE_Font::RE_Font(std::string file, int size)
{
	ptsize = size;
	font_file = file;

	loadfont();
}

RE_Font::~RE_Font()
{
	TTF_CloseFont(font);
	if(text_image != NULL)
		SDL_DestroyTexture(text_image);
}

void RE_Font::resize(int new_size)
{
	ptsize = new_size;
	loadfont();
}

void RE_Font::set_fg_color(char r, char g, char b, char a)
{
	fgColor.r = r;
	fgColor.g = g;
	fgColor.b = b;
	//fgColor.a = a;
}

void RE_Font::set_bg_color(char r, char g, char b, char a)
{
	bgColor.r = r;
	bgColor.g = g;
	bgColor.b = b;
	//bgColor.a = a;
}

SDL_Texture* RE_Font::set_text(std::string text)
{
	SDL_Surface *resulting_text = NULL;

    if (quality == solid) resulting_text = TTF_RenderUTF8_Solid(font, text.c_str(), fgColor);
    else if (quality == shaded) resulting_text = TTF_RenderUTF8_Shaded(font, text.c_str(), fgColor, bgColor);
    else if (quality == blended) resulting_text = TTF_RenderUTF8_Blended(font, text.c_str(), fgColor);

	if(resulting_text == NULL)
		std::cerr << "RE_Font: couldn't create a text surface:" << TTF_GetError() << "\n";

	if(text_image != NULL)
		SDL_DestroyTexture(text_image);

	//we want a fixed size for the resulting texture
	if(width > 0 && height > 0)
	{
		text_image = SDL_CreateTexture(gc->renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height);

		SDL_Texture *tmp_text = SDL_CreateTextureFromSurface(gc->renderer, resulting_text);

		int w;
		int h;

		SDL_QueryTexture(tmp_text, NULL, NULL, &w, &h);

		SDL_SetRenderTarget(gc->renderer, text_image);
		SDL_SetTextureColorMod( tmp_text, 255, 255, 255 );

		SDL_Rect dest;
		dest.x = 0;
		dest.y = 0;
		dest.w = width;
		dest.h = height;

		if(w < width)
			dest.w = w;
		if(h < height)
			dest.h = h;

		//clear default target
		SDL_SetRenderDrawColor(gc->renderer, bgColor.r, bgColor.g, bgColor.b, 255);
		SDL_RenderClear(gc->renderer);

		SDL_RenderCopy(gc->renderer, tmp_text, NULL, &dest);

		SDL_DestroyTexture(tmp_text);
	}
	else
	{
		//create the final texture (size dependent on the text)
		text_image = SDL_CreateTextureFromSurface(gc->renderer, resulting_text);
	}

	SDL_FreeSurface(resulting_text);

	if(text_image == NULL)
	{
		std::cerr << "RE_Font: couldn't create a texture to render:" << SDL_GetError() << "\n";
		return NULL;
 	}

	SDL_SetTextureAlphaMod(text_image, 255);
	SDL_SetTextureBlendMode(text_image, SDL_BLENDMODE_ADD);

	return text_image;
}

int RE_Font::get_text_height()
{
	int w;
	int h;
	int access;
	Uint32 format;

	SDL_QueryTexture(text_image,
                     &format,
                     &access,
                     &w,
                     &h);

	return h;
}

int RE_Font::get_text_width()
{
	int w;
	int h;
	int access;
	Uint32 format;

	SDL_QueryTexture(text_image,
                     &format,
                     &access,
                     &w,
                     &h);

	return w;
}

void RE_Font::draw(int x, int y)
{
	int w;
	int h;
	int access;
	Uint32 format;

	SDL_QueryTexture(text_image,
                     &format,
                     &access,
                     &w,
                     &h);

	SDL_Rect draw_rect;
	draw_rect.x = 0;
	draw_rect.y = 0;
	draw_rect.w = w;
	draw_rect.h = h;

	gc->draw(text_image, &draw_rect, x, y);
}

void RE_Font::draw_text(int x, int y, std::string text, int s_width, int s_height)
{
	width = s_width;
	height = s_height;
    set_text(text);
	draw(x,y);
}

void RE_Font::loadfont()
{
	set_bg_color(0, 0, 0, 0);
	set_fg_color(255, 255, 255, 255);

	quality = shaded;
	//quality = blended;

	if(font != NULL)
	{
		TTF_CloseFont(font);
		font = NULL;
	}

    font = TTF_OpenFont(font_file.c_str(), ptsize);
    if (font == NULL)
	{
      std:cerr << "Unable to load font (" << font_file <<") :" << TTF_GetError()<< "\n";
      // Handle the error here.
    }

	TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
}
