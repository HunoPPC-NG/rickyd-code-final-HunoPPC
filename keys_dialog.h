/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_keys_dialog
#define header_keys_dialog

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
#include "re_button.h"

class KeysDialog
{
public:
	enum DialogResponse
	{
		OK = 0,
		CANCEL,
		UNKNOWN
	};
//!Construction:
public:
	KeysDialog(RE_GameContext* pgc, RE_ResourceManager* resources, RE_Events* ev);

	~KeysDialog();

	void update(int time_elapsed);
	void show(int time_elapsed);
	bool is_commited(){return commited;}
	DialogResponse get_response(){return response;}

private:
	RE_GameContext*  gc;
	RE_ResourceManager* global_resources;
	void map_temp_key(RE_Events::KEY_CODE key, SDL_Keycode sdl_key);
	SDL_Keycode get_temp_key(RE_Events::KEY_CODE key);
	void set_key_map();
	void get_key_map();

	SDL_Keycode tempkeys[RE_Events::NUMBER_OF_KEYS];

	int key_menu_index;

	RE_Events* events;

	bool commited;
	DialogResponse response;

	RE_Point mouse_pointer;
	RE_Point gc_pointer;
	RE_Rect* up_box_rect;
	RE_Rect* down_box_rect;
	RE_Rect* left_box_rect;
	RE_Rect* right_box_rect;
	RE_Rect* action_box_rect;
	RE_Rect* pause_box_rect;
	RE_Rect* quit_box_rect;

	RE_Font *dialog_font;
	RE_Font *title_font;

	RE_Button* button_ok;
	RE_Button* button_cancel;

};
#endif
