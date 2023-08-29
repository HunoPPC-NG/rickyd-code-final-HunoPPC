/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_elevator
#define header_gameobject_elevator

#if _MSC_VER > 1000
#pragma once
#endif

#define ELEVATOR_V_VELOCITY 4
#define ELEVATOR_H_VELOCITY 4
#define ELEVATOR_WAIT 2000
#define ELEVATOR_CYCLES 10 //number of moves before waiting for trigger

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "re_rect.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Elevator : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active=0,
		waiting1, //wait for trigger
		waiting2, //wait to invert direction
	};

//!Construction:
public:
	GameObject_Elevator(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Elevator();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Elevator(world, object_element);
	}
//!Attributes:
public:
	virtual void update_collision_box();

//!Operations:
public:
	virtual void show(int view_x, int view_y);
	virtual bool turn(int time_elapsed);

	void deploy(int new_x, int new_y, int state=0) {return;}

	virtual int get_total() {return 0;}
	virtual void set_total(int total) {return;}

	virtual bool is_dead(){return i_am_dead;}

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_switch_box(int sb_x, int sb_y, int sb_width, int sb_height);

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state, bool final = false);

	ObjectState STATE;

	RE_Rect* TriggerBox; //trigger box
	RE_Rect* SwitchBox; //switch box

	bool check_trigger();
	bool check_switch();
	bool in_window(ObjectState state);

	int anim_pos;

	bool i_am_dead;

	//direction:
	// 0 left, 1 right, 2 up, 3 down
	int direction;
	int initial_direction;

	// 0 - not returning; 1 - returning (invert direction)
	int returning;

	int turn_time;
	int move_time;
	int wait_time1;
	int def_wait;
	int elevator_velocity;
	int elevator_return_velocity;
	int def_cycles;
	int cycles;
	int def_trigger_delay;
	int trigger_delay;
	bool first_trigger;

	RE_Point start;
	RE_Point end;

	ObjectState previous_STATE;

	//: Object sprites for each STATE.
	RE_Sprite* spr_object[3];

	//:Sounds
	RE_SoundBuffer* sfx_moving;

	RE_SoundSession* sound;
};

#endif
