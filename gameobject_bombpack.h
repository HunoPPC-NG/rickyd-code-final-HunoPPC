/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_bombpack
#define header_gameobject_bombpack

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Bombpack : public GameObject_Moving
{
//!Enums:
public:
	//: Different tile types in the game.
	enum ObjectState
	{
		active = 0,
		inactive
	};

//!Construction:
public:
	//: Construct a bombpack object
	GameObject_Bombpack(World *world,  tinyxml2::XMLElement* object_element);
	//: Destruct a bombpack object
	~GameObject_Bombpack();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element) 
	{ 
		return new GameObject_Bombpack(world, object_element); 
	}
//!Attributes:
public:
	//: Returns true if player got the powerup.
	virtual void update_collision_box();

//!Operations:
public:
	//: Draw player onto graphic context.
	virtual void show(int view_x, int view_y);

	//: Move it
	virtual bool turn(int time_elapsed);
	
	void deploy(int new_x, int new_y, int state=0){return;}
	
	virtual int get_total() {return 0;}
	virtual void set_total(int total) {return;}
	
	virtual bool is_dead(){return false;}
	
	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state);
	
	int total_bombs;
	
	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;
	
	int turn_time;
	
	//: Bombpack images for each STATE.
	RE_Sprite* spr_object[1];

	//: Sound effect played when bonus is collected.
	RE_SoundBuffer* sfx_bombpack;
	
	RE_SoundSession* sound;

};

#endif
