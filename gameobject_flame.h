/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_flame
#define header_gameobject_flame

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Flame : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		big_right = 0,
		big_left,
		small_right,
		small_left,
		waiting,
		inactive
	};

//!Construction:
public:
	//: Construct a flame object
	GameObject_Flame(World *world, tinyxml2::XMLElement* object_element);
	//: Destruct a flame object
	~GameObject_Flame();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Flame(world, object_element);
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

	int direction;
	int pos_x; //x initial position (for big/small left is right side)
	int pos_y;
	int anim_pos;

	RE_Rect* TriggerBox;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;

	//: Flame images for each STATE.
	RE_Sprite* spr_object[4];

	RE_SoundBuffer* sfx_flame;
	RE_SoundSession* sound;
};

#endif
