/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include "re_inputbox.h"
#include "tinyxml2.h"

RE_InputBox::RE_InputBox(RE_GameContext* pgc, std::string xml_path, RE_ResourceManager *resource, RE_Events* pevents)
	: RE_Resource(pgc, pevents) //pass the events to resource (adds input to the list)
{
	spr_background = new RE_Sprite(pgc, xml_path+"/box/sprite", resource);
	spr_cursor = new RE_Sprite(pgc, xml_path+"/cursor/sprite", resource);
	input_font = new RE_Font(pgc, xml_path+"/font", resource);

	box_rect = new RE_Rect(0, 0, spr_background->get_width(), spr_background->get_height());
	text="";
	max_input_chars = 14;

	set_commited(false);
	turn_time = 0;
	blink_time = 2*INPUTBOX_BLINK_TIME;
}

RE_InputBox::~RE_InputBox()
{
	delete(spr_background);
	delete(spr_cursor);
	delete(input_font);
	delete(box_rect);
}

void RE_InputBox::update(Uint32 time_elapsed)
{
	check_tab_event();

	if(!events->inputing_text)
		events->open_text_input();
	if(events->has_backspace() && text.size() > 0)
		text.erase(text.size() - 1);

	turn_time = time_elapsed;

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
			//std::cout << "mouse clicked in listbox up button\n";
			grab_focus();
		}
	}

	//std::cout << "input index "<< get_input_index() << " has focus set to " << has_focus() << "\n";
	if(has_focus())
	{
		text += events->get_input_text();
		if(text.size() > max_input_chars)
            		text.erase(max_input_chars);
		if(events->commit_text)
			set_commited(true);
	}
}

void RE_InputBox::draw(int x, int y)
{
	cursor_pos = x + 10;
	if(label.size() > 0)
	{
		input_font->draw_text(x, y, label);
		y += input_font->get_text_height(); //draw input box below label
	}

	box_rect->Left(x);
	box_rect->Top(y);
	box_rect->Right(x+spr_background->get_width());
	box_rect->Bottom(y + spr_background->get_height());

	if(has_focus())
		spr_background->set_frame(1);
	else
		spr_background->set_frame(0);
	spr_background->draw(x, y);

	//should also draw up and down buttons text if needed:  "\/" and "/\"

	//draw input box text
	if(text.size() > 0)
	{
		input_font->draw_text(cursor_pos, y + ((spr_background->get_height() - input_font->get_text_height())/2), text);
		cursor_pos += input_font->get_text_width();
	}
	//draw input box cursor if we have focus
	if(has_focus())
	{
		if(blink_time > INPUTBOX_BLINK_TIME)
		{
			spr_cursor->set_frame(0);
			spr_cursor->draw(cursor_pos, y);
		}
		else if(blink_time <= 0)
		{
			blink_time = 2*INPUTBOX_BLINK_TIME;
		}
		blink_time -= turn_time;
	}
}
