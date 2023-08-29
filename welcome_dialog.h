/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_welcome_dialog
#define header_welcome_dialog

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "re_levelmanager.h"
#include "re_resourcemanager.h"
#include "re_gamecontext.h"
#include "re_sprite.h"
#include "re_font.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "re_scores.h"
#include "re_listbox.h"
#include "re_button.h"

class WelcomeDialog
{
public:
	enum DialogResponse
	{
		PLAY = 0,
		KEYS,
		QUIT,
		UNKNOWN
	};
//!Construction:
public:
	WelcomeDialog(RE_GameContext* pgc, RE_ResourceManager* resources, RE_Events* ev, RE_LevelManager* lvm, int map_num = 1);

	~WelcomeDialog();

	void update(int time_elapsed);
	void show(int time_elapsed);
	bool is_commited(){return commited;}
	DialogResponse get_response(){return response;}
	int get_selected_map(){return sel_map;}

private:
	RE_GameContext*  gc;
	RE_ResourceManager* global_resources;
	RE_Events* events;
	RE_LevelManager* Levels;

	bool commited;
	DialogResponse response;
	int sel_map;

	RE_ListBox* level_menu;
	RE_Button* button_play;
	RE_Button* button_keys;
	RE_Button* button_quit;

};
#endif
