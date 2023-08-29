/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_welcome
#define header_welcome

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

#define WELCOME_TIME 8000
#define SCORE_TIME 5000

class Welcome
{
//!Construction:
public:
	Welcome(RE_GameContext* pgc, RE_ResourceManager* game_resources, Scores* tb_scores);

	~Welcome();

	void show(int time_elapsed);

private:
	RE_GameContext*  gc;
	int turn;

	int welcome_time;
	int score_time;

	Scores* scores;

	RE_Font *welcome_font;
	RE_Font *score_font;
	//RE_ResourceManager *local_resources;

	RE_Sprite *welcome_screen;

	RE_SoundBuffer* sfx_welcome;
	RE_SoundSession* sound;

};
#endif
