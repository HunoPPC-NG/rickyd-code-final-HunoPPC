/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_electricity
#define header_gameobject_electricity

#if _MSC_VER > 1000
#pragma once
#endif

#define ELECTRICITY_VELOCITY 10
#define ELECTRICITY_TRIGGER_TIME 1200 //ms

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Electricity : public GameObject_Moving
{
//!Enums:
public:
	//: Different tile types in the game.
	enum ObjectState
	{
		moving = 0,
		waiting1,
		waiting,
		inactive
	};

//!Construction:
public:
	//: Construct a object
	GameObject_Electricity(World *world, tinyxml2::XMLElement* object_element);
	//: Destruct a  object
	~GameObject_Electricity();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Electricity(world, object_element);
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

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_bounding_box(int bb_x, int bb_y, int bb_width, int bb_height);

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state);

	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;
	int move_time;
	int def_trigger_time;
	int trigger_time;
	int def_trigger_delay;
	int trigger_delay;
	bool first_trigger;
	int def_active_time;
	int active_time;

	int moving_dir; //-1 go left; +1 go right
	int initial_x;
	int initial_y;

	RE_Rect* TriggerBox; //trigger box
	RE_Rect* SwitchBox; //switch box
	RE_Rect* bounding_box;

	bool check_trigger();
	bool check_switch();

	//:images for each STATE.
	RE_Sprite* spr_object[1];

	//: Sound effect
	RE_SoundBuffer* sfx_electricity;
	RE_SoundSession* sound;

};

#endif
