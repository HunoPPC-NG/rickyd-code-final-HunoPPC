/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#ifndef header_re_listbox
#define header_re_listbox

#include "precomp.h"
#include "re_events.h"
#include "re_gamecontext.h"
#include "re_resource.h"
#include "re_resourcemanager.h"
#include "re_font.h"
#include "re_sprite.h"
#include "re_rect.h"

#define UP_DOWN_HEIGHT 20

class RE_ListBoxItem
{
public:
	std::string text;
	int value;
	bool enabled;
};

class RE_ListBox : public RE_Resource
{
public:
	RE_ListBox(RE_GameContext* pgc, std::string xml_path, RE_ResourceManager *resource, RE_Events* pevents);
	~RE_ListBox();
	void add_label(std::string new_label){label = new_label;}
	void add_item(RE_ListBoxItem new_item);
	void enable_item(int index, bool enable = true);
	void update(Uint32 time_elapsed = 0);
	void draw(int x, int y);
	std::string get_text(int index);
	int get_value(int index);
	int get_selected_index(){return selected_index;}
	int set_selected_index(int index);
	bool is_enabled(int index);

private:
	std::vector<RE_ListBoxItem> item_list;
	RE_Sprite* spr_background;
	int cursor_pos;
	RE_Font* input_font;
	std::string label;
	int selected_index;
	Uint32 turn_time;
	int scroll_item;
	int max_display_items;
	int item_height;
	RE_Point mouse_pointer;
	RE_Point gc_pointer;
	RE_Rect* up_box_rect;
	RE_Rect* down_box_rect;
	RE_Rect* list_box_rect;
};

#endif
