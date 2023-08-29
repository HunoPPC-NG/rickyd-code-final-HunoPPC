/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_falltrap
#define header_gameobject_falltrap

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

#define TRAP_TIME_TO_FALL   400
#define TRAP_TIME_TO_REARM  2500
#define TRAP_TIME_TO_TRIGGER  8000
#define TRAP_FALL_VELOCITY  10

class GameObject_Falltrap : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active = 0,
		waiting,
		waiting_switch,
		rearm,
		rearmed,
	};

//!Construction:
public:
	//: Construct a gatetrap object
	GameObject_Falltrap(World *world, tinyxml2::XMLElement* object_element);
	//: Destruct a gatetrap object
	~GameObject_Falltrap();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Falltrap(world, object_element);
	}
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

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state);

	bool check_trigger();
	bool check_switch();

	void set_switch_box(int sb_x, int sb_y, int sb_width, int sb_height);

	int anim_pos;
	int start_y;
	int end_y;
	int time_to_fall;
	int time_to_rearm;
	int time_to_trigger;
	int def_time_to_fall;
	int def_time_to_rearm;
	int def_time_to_trigger;
	int def_velocity;
	// 0 - kill_none (default)
	// 1 - wall
	int wait_type;

	RE_Rect* TriggerBox;
	RE_Rect* SwitchBox;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;

	//: Falltrap images for each STATE.
	RE_Sprite* spr_object[1];

	RE_SoundBuffer* sfx_moving;
	RE_SoundSession* sound;
};

#endif
