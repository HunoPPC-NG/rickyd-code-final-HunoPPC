/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_score_dialog
#define header_score_dialog

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
#include "re_inputbox.h"
#include "re_button.h"

class ScoreDialog
{
public:
	enum DialogResponse
	{
		OK = 0,
		UNKNOWN
	};
//!Construction:
public:
	ScoreDialog(RE_GameContext* pgc, RE_ResourceManager* resources, RE_Events* ev, Scores* sc, int score_value = 0);

	~ScoreDialog();

	void update(int time_elapsed);
	void show(int time_elapsed);
	bool is_commited(){return commited;}
	DialogResponse get_response(){return response;}

private:
	RE_GameContext*  gc;
	RE_ResourceManager* global_resources;
	RE_Events* events;
	Scores* scores;

	int score;

	bool commited;
	DialogResponse response;

	RE_Font* title_font;
	RE_Font* dialog_font;

	RE_InputBox* input_box;
	RE_Button* button_save;

};
#endif
