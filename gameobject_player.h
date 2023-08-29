/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_player
#define header_gameobject_player

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_sprite.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"

#define PLAYER_VELOCITY 4
#define OVERBIKE_VELOCITY 6
#define PLAYER_JUMP_VELOCITY 12.5f
#define PLAYER_STICK_TIME 70

class GameObject_Player : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		stand_left = 0,
		stand_right,
		walk_left,
		walk_right,
		crawl_left,
		crawl_right,
		duck_right,
		duck_left,
		jump_left,
		jump_right,
		climb,
		climb_stop,
		shoot_left,
		shoot_right,
		hammer_left,
		hammer_right,
		overbike_left,
		overbike_right,
		dead,
		inactive
	};

//!Construction:
public:
	//: Construct a player
	GameObject_Player(World* world);
	//: Destruct
	~GameObject_Player();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Player(world);
	}
//!Attributes:
public:
	virtual void update_collision_box();

//!Operations:
public:
	//: Draw player onto graphic context.
	virtual void show(int view_x, int view_y);

	//: Move player.
	virtual bool turn(int time_elapsed);

	//: deploy
	virtual void deploy(int start_x, int start_y, int state);

	virtual int get_total(){return 0;}
	virtual void set_total(int total){return;}

	bool is_switch_trigger(RE_Rect* switch_button);
	bool is_stick(RE_Rect* object_box);
	//: Kill the player.
	void you_are_dead() { i_am_dead = true; }
	virtual bool is_dead() {return i_am_dead;}

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

//!Implementation:
private:

	void update_STATE();

	bool play_sound();

	void set_state(int state);

	int stick_time; // 0> and in hammer state stuns enemies

	int scroll_mode; //1- rick1 mode;  2- rick2 mode (center player)

	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;

	void set_STATE(ObjectState state, bool final = false);

	int turn_time;

	CollisionBox SB; //stand sprite collision box (for can_stand)

	bool is_on_jump_platform();

	bool i_am_dead;

	//: Player images for each STATE.
	RE_Sprite* spr_object[20];

	//:Sounds
	RE_SoundBuffer* sfx_overbike;
	RE_SoundBuffer* sfx_walk;
	RE_SoundBuffer* sfx_crawl;
	RE_SoundBuffer* sfx_jump;
	RE_SoundBuffer* sfx_dead;

	RE_SoundSession* sound;
};

#endif
