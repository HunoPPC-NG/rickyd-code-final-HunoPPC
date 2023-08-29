/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "welcome_dialog.h"

WelcomeDialog::WelcomeDialog(RE_GameContext* pgc, RE_ResourceManager* resources, RE_Events* ev, RE_LevelManager* lvm, int map_num)
{
	gc = pgc;
	global_resources = resources;
	events = ev;
	Levels = lvm;

	level_menu = new RE_ListBox(gc, "global/objects/listbox", global_resources, events);
	level_menu->add_label("Stages");

	//add level list
	int n_map = 1;
	for(n_map = 1; n_map <= Levels->get_num_maps(); n_map++)
	{
		RE_ListBoxItem level_item;

		std::ostringstream levelSStream;
		levelSStream <<  n_map << " - " << Levels->get_map_name(n_map);
		if(!(Levels->is_map_locked(n_map)))
			levelSStream << " (unlocked)";
		else
			levelSStream << " (locked)";

		level_item.text = levelSStream.str();
		level_item.value = n_map;
		level_item.enabled = !(Levels->is_map_locked(n_map));
		level_menu->add_item(level_item);
		if(n_map == map_num)
			level_menu->set_selected_index(n_map-1);
	}

	button_play = new RE_Button(gc, "global/objects/button", global_resources, events);
	button_play->add_label("Play");

	button_keys = new RE_Button(gc, "global/objects/button", global_resources, events);
	button_keys->add_label("Keys");

	button_quit = new RE_Button(gc, "global/objects/button", global_resources, events);
	button_quit->add_label("Quit");

	level_menu->grab_focus(); //give focus to list box
	commited = false;
	response = UNKNOWN;
	sel_map = map_num;
}

WelcomeDialog::~WelcomeDialog()
{
	delete(level_menu);
	delete(button_play);
	delete(button_keys);
	delete(button_quit);
}

void WelcomeDialog::update(int time_elapsed)
{
	level_menu->update(time_elapsed);
	button_play->update(time_elapsed);
	button_keys->update(time_elapsed);
	button_quit->update(time_elapsed);

	if( level_menu->is_commited() ||
		button_play->is_commited())
	{
		sel_map = level_menu->get_value(level_menu->get_selected_index());
		if(!(Levels->is_map_locked(sel_map)))
		{
			commited = true;
			response = PLAY;
		}
		else // invalid level
		{
			//uncommit inputs
			level_menu->set_commited(false);
			button_play->set_commited(false);
			//set focus to listbox
			level_menu->grab_focus();
		}
	}
	else if(button_keys->is_commited())
	{
		commited = true;
		response = KEYS;
	}
	else if(button_quit->is_commited())
	{
		commited = true;
		response = QUIT;
	}
}

void WelcomeDialog::show(int time_elapsed)
{
	level_menu->draw(180, 260);
	button_quit->draw(180, 430);
	button_keys->draw(290, 430);
	button_play->draw(400, 430);
}
