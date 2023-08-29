/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_intro
#define header_intro

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

class Intro
{
//!Construction:
public:
	Intro(RE_GameContext*  pgc, RE_LevelManager* levels, int map=1);

	~Intro();

	void show(int time_elapsed);

private:
	RE_GameContext*  gc;
	int intro_turn;
	int intro_strip_turn;
	int intro_frame_ind; //clip
	int intro_frame2_ind; //strip 
	
	RE_Font *intro_font;
	
	RE_ResourceManager *intro_resources;
	
	RE_Sprite *intro_clip;
	RE_Sprite *intro_strip;
	
	RE_SoundBuffer* sfx_intro;
	RE_SoundSession* sound;
};
#endif
