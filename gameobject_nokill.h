/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_nokill
#define header_gameobject_nokill

#if _MSC_VER > 1000
#pragma once
#endif

#define NOKILL_VELOCITY 4
#define NOKILL_REST_TIME 1500
#define NOKILL_WALK_TIME 1800

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Nokill : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		walk_right =0,
		walk_left,
		resting,
		inactive,
	};

//!Construction:
public:
	GameObject_Nokill(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Nokill();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Nokill(world, object_element);
	}
//!Attributes:
public:
	virtual void update_collision_box();

//!Operations:
public:
	virtual void show(int view_x, int view_y);
	virtual bool turn(int time_elapsed);

	void deploy(int new_x, int new_y, int state=0);

	virtual int get_total() {return 0;}
	virtual void set_total(int total) {return;}

	virtual bool is_dead(){return i_am_dead;}

	virtual int get_width() {return spr_object[STATE]->get_width();}
	virtual int get_height(){return spr_object[STATE]->get_height();}

//!Implementation:
private:
	void update_STATE();

	ObjectState STATE;
	void set_STATE(ObjectState state, bool final = false);
	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_bounding_box(int bb_x, int bb_y, int bb_width, int bb_height);

	bool check_trigger();
	bool in_window(ObjectState state);

	RE_Rect* TriggerBox;
	RE_Rect* bounding_box;

	bool i_am_dead;

	int def_rest_time;
	int def_walk_time;
	int rest_time;
	int walk_time;

	int default_speed1; //anim speed moving
	int default_speed2; //anim speed resting

	int anim_pos;

	int turn_time;
	int move_time;

	int direction;

	ObjectState previous_STATE;

	//: Object sprites for each STATE.
	RE_Sprite* spr_object[3];

};

#endif
