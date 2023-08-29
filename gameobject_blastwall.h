/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_blastwall
#define header_gameobject_blastwall

#if _MSC_VER > 1000
#pragma once
#endif

#define BLOCK_VELOCITY 10

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Blastwall : public GameObject_Moving
{
//!Enums:
public:
	//: Different tile types in the game.
	enum ObjectState
	{
		active=0,
		blasting,
		moving,
		inactive
	};

//!Construction:
public:
	//: Construct a player at position (x,y).
	GameObject_Blastwall(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	~GameObject_Blastwall();

//!Attributes:
public:
	//: Returns true if player got the powerup.
	virtual void update_collision_box();

//!Operations:
public:
	//: Draw player onto graphic context.
	virtual void show(int view_x, int view_y);

	//: Move player.
	virtual bool turn(int time_elapsed);

	void deploy(int new_x, int new_y, int state=0) {return;}

	virtual int get_total() {return 0;}
	virtual void set_total(int total) {return;}

	virtual bool is_dead(){return i_am_dead;}

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height);
	bool in_window(ObjectState state);

//!Implementation:
private:
	std::string obj_name;
	void update_STATE();
	void set_state(int state);

	ObjectState STATE;
	void set_STATE(ObjectState state, bool final = false);

	int anim_pos;

	bool i_am_dead;

	//-2 down
	//-1 up
	//0 no movement
	// 1 right
	// 2 left (def)
	int direction;

	int turn_time;

	RE_Rect* TriggerBox; //trigger box
	RE_Rect* SwitchBox; //switch box

	bool check_trigger();
	bool check_switch();

	ObjectState previous_STATE;

	//: Object sprites for each STATE.
	RE_Sprite* spr_object[3];

	//:Sounds
	RE_SoundBuffer* sfx_moving;

	RE_SoundSession* sound;
};

#endif
