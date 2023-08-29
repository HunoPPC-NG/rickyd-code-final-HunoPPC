/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_jumpplatform
#define header_gameobject_jumpplatform

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_JumpPlatform : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active = 0,
		inactive
	};

//!Construction:
public:
	//: Construct a jumpplatform object
	GameObject_JumpPlatform(World *world,  tinyxml2::XMLElement* object_element);
	//: Destruct a jumpplatform object
	~GameObject_JumpPlatform();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element) 
	{ 
		return new GameObject_JumpPlatform(world, object_element); 
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
	
	virtual int get_width() {return platform_width;}
	virtual int get_height(){return platform_height;}

//!Implementation:
private:
	int platform_width;
	int platform_height;
	void set_STATE(ObjectState state);
	void update_STATE();
	
	ObjectState STATE;
	ObjectState previous_STATE;
	
	//this object has no sprite image (it's just a rectangular area)
	
	//: Sound effect
	//RE_SoundBuffer* sfx_jumpplatform;
	
	//RE_SoundSession* sound;

};

#endif
