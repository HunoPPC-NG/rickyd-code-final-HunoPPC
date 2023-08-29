/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_bat
#define header_gameobject_bat

#if _MSC_VER > 1000
#pragma once
#endif

#define BAT_VELOCITY 4
#define WAIT_TIME  2000 //milisec

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Bat : public GameObject_Moving
{
//!Enums:
public:
	//: Different tile types in the game.
	enum ObjectState
	{
		resting=0,
		moving
	};

//!Construction:
public:
	//: Construct a player at position (x,y).
	GameObject_Bat(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Bat();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element) 
	{ 
		return new GameObject_Bat(world, object_element); 
	}
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
	
	bool i_am_dead;
	
	int wait_time;
	int anim_pos;
	
	int turn_time;
	int move_time;
	
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	
	float yvel;
	float xvel;
	
	ObjectState previous_STATE;
	
	//: Object sprites for each STATE.
	RE_Sprite* spr_object[2];

};

#endif
