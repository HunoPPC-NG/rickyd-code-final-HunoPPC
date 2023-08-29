/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_statickiller
#define header_gameobject_statickiller

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_StaticKiller : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		right = 0,
		left,
		up,
		down,
		inactive
	};

//!Construction:
public:
	GameObject_StaticKiller(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	~GameObject_StaticKiller();
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

	virtual int get_width() {return spr_object[STATE]->get_width();}
	virtual int get_height(){return spr_object[STATE]->get_height();}

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state);
	bool in_window(ObjectState state);

	std::string obj_name;

	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;

	//: Spikes images for each STATE.
	RE_Sprite* spr_object[4];
};

#endif
