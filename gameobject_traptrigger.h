/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_traptrigger
#define header_gameobject_traptrigger

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Traptrigger : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active_up = 0,
		active_down,
		active_left,
		active_right,
		waiting,
		inactive
	};

//!Construction:
public:
	GameObject_Traptrigger(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	~GameObject_Traptrigger();
//!Attributes:
public:
	virtual void update_collision_box();

//!Operations:
public:
	virtual void show(int view_x, int view_y);
	virtual bool turn(int time_elapsed);

	void deploy(int new_x, int new_y, int state=0){return;}

	virtual int get_total() {return 0;}
	virtual void set_total(int total) {return;}

	virtual bool is_dead(){return false;}

	virtual int get_width() {return 0;}
	virtual int get_height(){return 0;}

	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_trigger_box_2(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height);

//!Implementation:
private:
	std::string obj_name;
	void update_STATE();
	void set_state(int state);
	void set_STATE(ObjectState state);
	bool in_window(ObjectState state);

	int turn_time;
	int anim_pos;
	int time_to_rearm;
	int def_rearm_time;

	int initial_x;
	int initial_y;
	// 1 - switch sets it off (default)
	// 0 - switch sets it on
	int switch_state;

	// 2 - trigger sets it off but only bullet can trigger
	// 1 - trigger sets it off
	// 0 - trigger sets it on (default)
	// -1 - trigger sets it but only player can trigger (not bullet)
	int trigger_state;

	//0: active up (default)
	//1: active down
	//2: active left
	//3: active right
	int initial_state;
	bool is_on;

	//if TriggerBox != NULL  state = waiting
	RE_Rect* TriggerBox; //trigger box
	RE_Rect* TriggerBox2; //second trigger box
	RE_Rect* SwitchBox; //switch box

	bool check_trigger();
	bool check_switch();

	ObjectState STATE;
	ObjectState previous_STATE;

	//: Traptrigger images for each STATE.
	RE_Sprite* spr_object[4];

	RE_SoundBuffer* sfx_traptrigger;
	RE_SoundSession* sound;
};

#endif
