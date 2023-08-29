/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#ifndef header_re_inputbox
#define header_re_inputbox

#include "precomp.h"
#include "re_events.h"
#include "re_gamecontext.h"
#include "re_resource.h"
#include "re_resourcemanager.h"
#include "re_font.h"
#include "re_sprite.h"
#include "re_rect.h"

#define INPUTBOX_BLINK_TIME 200 //in ms

class RE_InputBox : public RE_Resource
{
public:
	RE_InputBox(RE_GameContext* pgc, std::string xml_path, RE_ResourceManager *resource, RE_Events* pevents);
	~RE_InputBox();
	void set_label(std::string new_label){label = new_label;}
	void update(Uint32 time_elapsed = 0);
	void draw(int x, int y);
	std::string get_text(){return text;}

private:
	RE_Sprite* spr_background;
	RE_Sprite* spr_cursor;
	RE_Font* input_font;
	RE_Point mouse_pointer;
	RE_Point gc_pointer;
	RE_Rect* box_rect;
	std::string text;
	std::string label;
	int max_input_chars;
	int cursor_pos;
	Uint32 turn_time;
	int blink_time;
};

#endif
