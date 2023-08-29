/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#ifndef header_re_button
#define header_re_button

#include "precomp.h"
#include "re_events.h"
#include "re_gamecontext.h"
#include "re_resource.h"
#include "re_resourcemanager.h"
#include "re_font.h"
#include "re_sprite.h"
#include "re_rect.h"


class RE_Button : public RE_Resource
{
public:
	RE_Button(RE_GameContext* pgc, std::string xml_path, RE_ResourceManager *resource, RE_Events* pevents);
	~RE_Button();
	void add_label(std::string new_label){label = new_label;}
	void update(Uint32 time_elapsed = 0);
	void draw(int x, int y);

private:
	RE_Sprite* spr_background;
	RE_Font* input_font;
	std::string label;
	Uint32 turn_time;

	RE_Point mouse_pointer;
	RE_Point gc_pointer;
	RE_Rect* box_rect;
};

#endif
