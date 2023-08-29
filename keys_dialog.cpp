/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "keys_dialog.h"

KeysDialog::KeysDialog(RE_GameContext* pgc, RE_ResourceManager* resources, RE_Events* ev)
{
	gc = pgc;
	global_resources = resources;
	events = ev;

	dialog_font = new RE_Font(gc, "global/objects/dialog/font", global_resources);
	title_font = new RE_Font(gc, "global/objects/dialog/font", global_resources);

	button_cancel = new RE_Button(gc, "global/objects/button", global_resources, events);
	button_cancel->add_label("Cancel");

	button_ok = new RE_Button(gc, "global/objects/button", global_resources, events);
	button_ok->add_label("OK");

	button_cancel->grab_focus();

	key_menu_index = 0;

	commited = false;
	response = UNKNOWN;
	//get current key mappings
	get_key_map();

	//create boxes for key entries
	up_box_rect = new RE_Rect(360,90,460,120);
	down_box_rect = new RE_Rect(360,120,460,150);
	left_box_rect = new RE_Rect(360,150,460,180);
	right_box_rect = new RE_Rect(360,180,460,210);
	action_box_rect = new RE_Rect(360,210,460,240);
	pause_box_rect = new RE_Rect(360,240,460,270);
	quit_box_rect = new RE_Rect(360,270,460,300);
}

KeysDialog::~KeysDialog()
{
	delete(dialog_font);
	delete(title_font);
	delete(button_cancel);
	delete(button_ok);
	delete(up_box_rect);
	delete(down_box_rect);
	delete(left_box_rect);
	delete(right_box_rect);
	delete(action_box_rect);
	delete(pause_box_rect);
	delete(quit_box_rect);
}

void KeysDialog::map_temp_key(RE_Events::KEY_CODE key, SDL_Keycode sdl_key)
{
	tempkeys[key] = sdl_key;
}

SDL_Keycode KeysDialog::get_temp_key(RE_Events::KEY_CODE key)
{
	return tempkeys[key];
}

void KeysDialog::get_key_map()
{

	map_temp_key(RE_Events::KEY_UP, events->get_key_map(RE_Events::KEY_UP));
	map_temp_key(RE_Events::KEY_DOWN, events->get_key_map(RE_Events::KEY_DOWN));
	map_temp_key(RE_Events::KEY_LEFT, events->get_key_map(RE_Events::KEY_LEFT));
	map_temp_key(RE_Events::KEY_RIGHT, events->get_key_map(RE_Events::KEY_RIGHT));
	map_temp_key(RE_Events::KEY_ACTION, events->get_key_map(RE_Events::KEY_ACTION));
	map_temp_key(RE_Events::KEY_PAUSE, events->get_key_map(RE_Events::KEY_PAUSE));
	map_temp_key(RE_Events::KEY_QUIT, events->get_key_map(RE_Events::KEY_QUIT));
}

void KeysDialog::set_key_map()
{
	events->set_key_map(RE_Events::KEY_UP, get_temp_key(RE_Events::KEY_UP));
	events->set_key_map(RE_Events::KEY_DOWN, get_temp_key(RE_Events::KEY_DOWN));
	events->set_key_map(RE_Events::KEY_LEFT, get_temp_key(RE_Events::KEY_LEFT));
	events->set_key_map(RE_Events::KEY_RIGHT, get_temp_key(RE_Events::KEY_RIGHT));
	events->set_key_map(RE_Events::KEY_ACTION, get_temp_key(RE_Events::KEY_ACTION));
	events->set_key_map(RE_Events::KEY_PAUSE, get_temp_key(RE_Events::KEY_PAUSE));
	events->set_key_map(RE_Events::KEY_QUIT, get_temp_key(RE_Events::KEY_QUIT));

}

void KeysDialog::update(int time_elapsed)
{
	button_cancel->update(time_elapsed);
	button_ok->update(time_elapsed);

	SDL_Keycode key_pressed = events->listen_for_key();

	if(key_pressed != 0)
	{
		std::string key_name = SDL_GetKeyName(key_pressed);
		if(key_name.size() <= 0)
		{
			std::cerr << "invalid key\n";
			return;
		}

		switch(key_menu_index)
		{
			case 0:
				map_temp_key(RE_Events::KEY_UP, key_pressed);
				break;
			case 1:
				map_temp_key(RE_Events::KEY_DOWN, key_pressed);
				break;
			case 2:
				map_temp_key(RE_Events::KEY_LEFT, key_pressed);
				break;
			case 3:
				map_temp_key(RE_Events::KEY_RIGHT, key_pressed);
				break;
			case 4:
				map_temp_key(RE_Events::KEY_ACTION, key_pressed);
				break;
			case 5:
				map_temp_key(RE_Events::KEY_PAUSE, key_pressed);
				break;
			case 6:
				map_temp_key(RE_Events::KEY_QUIT, key_pressed);
				break;
		}
		key_menu_index++;
		if(key_menu_index > 6)
		{
			button_ok->grab_focus();
			key_menu_index = 0;
		}
	}

	if(button_ok->is_commited())
	{
		//stop key listening: enable key press flags
		events->stop_listen_for_key();

		set_key_map();

		commited = true;
		response = OK;
	}
	else if(button_cancel->is_commited())
	{
		//stop key listening: enable key press flags
		events->stop_listen_for_key();
		commited = true;
		response = CANCEL;
	}
	else if(events->check_mouse_click(&mouse_pointer))
	{
		//mouse_pointer only works when window is not deformed(window = gc)
		//so we need to convert it from window to gc resolution
		double width_factor = (double) gc->get_width()/gc->get_window_width();
		double height_factor = (double) gc->get_height()/gc->get_window_height();

		//convert mouse position(window) to gc resolution
		gc_pointer.x = (int) mouse_pointer.x * width_factor;
		gc_pointer.y = (int) mouse_pointer.y * height_factor;

		//check colision with up_box
		if(gc_pointer.is_inside(up_box_rect))
		{
			key_menu_index = 0;
		}
		else if(gc_pointer.is_inside(down_box_rect))
		{
			key_menu_index = 1;
		}
		else if(gc_pointer.is_inside(left_box_rect))
		{
			key_menu_index = 2;
		}
		else if(gc_pointer.is_inside(right_box_rect))
		{
			key_menu_index = 3;
		}
		else if(gc_pointer.is_inside(action_box_rect))
		{
			key_menu_index = 4;
		}
		else if(gc_pointer.is_inside(pause_box_rect))
		{
			key_menu_index = 5;
		}
		else if(gc_pointer.is_inside(quit_box_rect))
		{
			key_menu_index = 6;
		}
	}
}

void KeysDialog::show(int time_elapsed)
{
	if(title_font->get_font_size() != 24)
		title_font->resize(24);
	title_font->set_fg_color(0xe8, 0x9f, 0x11, 0x00); //e89f11
	title_font->draw_text(250, 20, "KEYS");

	if(dialog_font->get_font_size() != 14)
		dialog_font->resize(14);

	int  i = 0;
	dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180, 100, "UP...........................");
	if(key_menu_index == i)
	{
		dialog_font->set_fg_color(0xf2,0xd4,0xf3,0);
		dialog_font->set_bg_color(100,10,100,0);
	}
	else
		dialog_font->set_fg_color(0xa2,0xd4,0x53,0);

	std::string key_name_up = SDL_GetKeyName(get_temp_key(RE_Events::KEY_UP));
	dialog_font->draw_text(360,100, key_name_up);

	i++;
	dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180, 130, "DOWN.....................");
	if(key_menu_index == i)
	{
		dialog_font->set_fg_color(0xf2,0xd4,0xf3,0);
		dialog_font->set_bg_color(100,10,100,0);
	}
	else
		dialog_font->set_fg_color(0xa2,0xd4,0x53,0);

	std::string key_name_down = SDL_GetKeyName(get_temp_key(RE_Events::KEY_DOWN));
	dialog_font->draw_text(360,130, key_name_down);

	i++;
	dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180, 160, "LEFT......................");
	if(key_menu_index == i)
	{
		dialog_font->set_fg_color(0xf2,0xd4,0xf3,0);
		dialog_font->set_bg_color(100,10,100,0);
	}
	else
		dialog_font->set_fg_color(0xa2,0xd4,0x53,0);


	dialog_font->draw_text(360,160, SDL_GetKeyName(get_temp_key(RE_Events::KEY_LEFT)));

	i++;
	dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180,  190, "RIGHT.....................");
	if(key_menu_index == i)
	{
		dialog_font->set_fg_color(0xf2,0xd4,0xf3,0);
		dialog_font->set_bg_color(100,10,100,0);
	}
	else
		dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->draw_text(360, 190, SDL_GetKeyName(get_temp_key(RE_Events::KEY_RIGHT)));

	i++;
	dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180,  220, "ACTION...................");
	if(key_menu_index == i)
	{
		dialog_font->set_fg_color(0xf2,0xd4,0xf3,0);
		dialog_font->set_bg_color(100,10,100,0);
	}
	else
		dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->draw_text(360, 220, SDL_GetKeyName(get_temp_key(RE_Events::KEY_ACTION)));

	i++;
	dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180, 250, "PAUSE.....................");
	if(key_menu_index == i)
	{
		dialog_font->set_fg_color(0xf2,0xd4,0xf3,0);
		dialog_font->set_bg_color(100,10,100,0);
	}
	else
		dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->draw_text(360, 250, SDL_GetKeyName(get_temp_key(RE_Events::KEY_PAUSE)));

	i++;
	dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180, 280, "QUIT........................");
	if(key_menu_index == i)
	{
		dialog_font->set_fg_color(0xf2,0xd4,0xf3,0);
		dialog_font->set_bg_color(100,10,100,0);
	}
	else
		dialog_font->set_fg_color(0xa2,0xd4,0x53,0);
	dialog_font->draw_text(360, 280, SDL_GetKeyName(get_temp_key(RE_Events::KEY_QUIT)));

	dialog_font->set_fg_color(0xEE,0xd4,0xEE,0);
	dialog_font->set_bg_color(0,0,0,0);
	dialog_font->draw_text(180, 330, "BULLET................       UP + ACTION");
	dialog_font->draw_text(180, 360, "BOMB...................       DOWN + ACTION  [+ LEFT|RIGHT]");

	button_cancel->draw(210, 400);
	button_ok->draw(310, 400);
}
