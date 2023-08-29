/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_spikerock
#define header_gameobject_spikerock

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Spikerock : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active=0,
		blasted
	};

//!Construction:
public:
	GameObject_Spikerock(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Spikerock();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element) 
	{ 
		return new GameObject_Spikerock(world, object_element); 
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
	
//!Implementation:
private:
	void update_STATE();
	void set_state(int state);
	
	ObjectState STATE;
	void set_STATE(ObjectState state, bool final = false);
	
	int anim_pos;

	bool i_am_dead;
	
	int direction; //0 left, 1 right
	
	int turn_time;
	
	ObjectState previous_STATE;
	
	//: Object sprites for each STATE.
	RE_Sprite* spr_object[2];
	
};

#endif
