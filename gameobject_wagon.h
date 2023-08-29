/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_wagon
#define header_gameobject_wagon

#if _MSC_VER > 1000
#pragma once
#endif

#define WAGON_MAX_VELOCITY 8
#define WAGON_INC_VELOCITY 2
#define WAGON_WAIT 1000

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "re_rect.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Wagon : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active=0,
		waiting1,
		waiting2,
		inactive
	};

//!Construction:
public:
	GameObject_Wagon(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Wagon();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Wagon(world, object_element);
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

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state, bool final = false);
	bool check_trigger();

	ObjectState STATE;

	RE_Rect* TriggerBox;

	int anim_pos;

	bool i_am_dead;

	int direction; //0 left, 1 right

	int def_max_velocity;
	int def_inc_velocity;
	int def_wait_time;

	int turn_time;
	int wait_time1;
	int wait_time2;

	int start_x;
	int end_x;
	ObjectType def_type;

	ObjectState previous_STATE;

	//: Object sprites for each STATE.
	RE_Sprite* spr_object[1];

	//:Sounds
	RE_SoundBuffer* sfx_moving;

	RE_SoundSession* sound;
};

#endif
