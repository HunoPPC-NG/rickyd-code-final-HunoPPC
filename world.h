/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_world
#define header_world

#if _MSC_VER > 1000
#pragma once
#endif

#define MAX_LIFES 6
#define GAME_FPS 35

#include "precomp.h"
#include <list>
#include "re_levelmanager.h"
#include "re_resourcemanager.h"
#include "re_gamecontext.h"
#include "re_sprite.h"
#include "re_point.h"
#include "re_font.h"
#include "re_events.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "gameobject.h"
#include "welcome.h"
#include "welcome_dialog.h"
#include "keys_dialog.h"
#include "score_dialog.h"
#include "re_scores.h"
#include "intro.h"
#include "map.h"

class Bonus
{
public:
	int map;
	int submap;
	Uint32 id;
	bool active;
};

class World
{
//!Construction:
public:
	World( RE_GameContext* pgc, RE_ResourceManager* resource, int start_map=1, int start_submap=1);

	~World();

//!Attributes:
public:
	RE_ResourceManager* global_resources; //object resources
	RE_ResourceManager* game_resources;   //game resources (resolution, fonts, ..)
	RE_ResourceManager* map_resources;
	RE_GameContext*  gc;
	RE_LevelManager* Levels;

	Map* map;

	std::list<GameObject*> objects;

	Scores* scores;

	GameObject *player;
	GameObject *bomb;
	GameObject *bullet;

	int scroll_x;
	int scroll_y;
	int next_scroll_x;
	int next_scroll_y;

	int location_index;

	int c4a;

	bool quit;

//!Operations:
public:
	void run();
	Intro *intro;
	void add_score(int bonus_score);

	void add_bonus(Uint32 id);
	void disable_bonus(Uint32 id);
	bool check_bonus(Uint32 id);

	GameObject* check_collision( GameObject::ObjectType type,
								 GameObject* owner=NULL);
	GameObject* check_point_collision(  GameObject::ObjectType type,
										RE_Point* point,
										GameObject* obj=NULL,
										bool reset_iter = true);
	GameObject* check_box_collision(GameObject::ObjectType type,
									RE_Rect* box,
									GameObject* obj=NULL,
									bool reset_iter = true);
	bool change_level;
	int connection_index;

//!Slots:
public:
	RE_Events* events;
	void on_quit() { quit = true; }

//!Implementation:
private:
	void reset_level(bool global_change = false);

	std::list<Bonus> bonus_list;

	int score;

	int view_x, view_y;

	int lifes;
	int map_num;
	int submap_num;

	int curr_map;    //you already reached curr_map (only increases)
	int curr_submap; //you already reached curr_submap (only increases)
	int curr_location; //entry position in curr_submap
	bool game_over;
	bool intro_shown;
	bool welcome_shown;
	bool process_score;
	bool process_keys;

	std::string data_dir;

	Welcome *welcome;

	RE_Font* score_font;
	RE_Font* menu_font;
	RE_Font* paused_font;
	RE_Font* gameover_font;

	RE_Sprite* spr_score_back;
	RE_Sprite* spr_score_bombs;
	RE_Sprite* spr_score_bullets;
	RE_Sprite* spr_score_lifes;

	KeysDialog* keys_dialog;
	WelcomeDialog* welcome_dialog;
	ScoreDialog* score_dialog;

	RE_SoundBuffer* sfx_submap;
	RE_SoundBuffer* sfx_gameover;
	RE_SoundSession* sound;
};

#endif
