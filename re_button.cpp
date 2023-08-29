/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include "re_button.h"
#include "tinyxml2.h"

RE_Button::RE_Button(RE_GameContext* pgc, std::string xml_path, RE_ResourceManager *resource, RE_Events* pevents)
	: RE_Resource(pgc, pevents) //pass the events to resource
{
	spr_background = new RE_Sprite(gc, xml_path+"/box/sprite", resource);
	input_font = new RE_Font(gc, xml_path+"/font", resource);

	label="Ok";

	box_rect = new RE_Rect(0, 0, spr_background->get_width(), spr_background->get_height());
	set_commited(false);
	turn_time = 0;
}

RE_Button::~RE_Button()
{
	delete(spr_background);
	delete(input_font);
	delete(box_rect);
}

void RE_Button::update(Uint32 time_elapsed)
{
	check_tab_event();

	turn_time = time_elapsed;

	if(has_focus() && events->keys[RE_Events::KEY_RETURN] )
	{
		set_commited(true);
		events->keys[RE_Events::KEY_RETURN] = false;
	}
	//mouse_pointer only works when window is not deformed(window = gc)
	//so we need to convert it from window to gc resolution
	if(events->check_mouse_click(&mouse_pointer))
	{
		//double width_factor = (double) gc->get_width()/gc->get_window_width();
		//double height_factor = (double) gc->get_height()/gc->get_window_height();
		double width_factor = 1;
		double height_factor = 1;

		//convert mouse position(window) to gc resolution
		gc_pointer.x = (int) mouse_pointer.x * width_factor;
		gc_pointer.y = (int) mouse_pointer.y * height_factor;

		//check colision with up_box
		if(gc_pointer.is_inside(box_rect))
		{
			grab_focus();
			//std::cout << "mouse clicked in button\n";
			set_commited(true);
		}
	}
}

void RE_Button::draw(int x, int y)
{
	box_rect->Left(x);
	box_rect->Top(y);
	box_rect->Right(x+spr_background->get_width());
	box_rect->Bottom(y + spr_background->get_height());

	if(label.size() > 0)
	{
		if(has_focus())
			input_font->set_fg_color(0x00,0xFF,0x00,0);
		else
			input_font->set_fg_color(0xa2,0xd4,0x53,0);

		input_font->set_bg_color(0,0,0,0);
		input_font->set_text(label);
		int cursor_pos_x = box_rect->Left() + spr_background->get_width()/2 - input_font->get_text_width()/2;
		int cursor_pos_y = y + spr_background->get_height()/2 - input_font->get_text_height()/2;

		input_font->draw(cursor_pos_x, cursor_pos_y);
	}

	if(has_focus())
		spr_background->set_frame(1);
	else
		spr_background->set_frame(0);
	spr_background->draw(x, y);
}
