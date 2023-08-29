/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_background
#define header_gameobject_background

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Background : public GameObject_Moving
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
	//: Construct a background object
	GameObject_Background(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	//: Destruct a background object
	~GameObject_Background();
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

//!Implementation:
private:
	int anim_pos;
	std::string obj_name;

	bool in_window(ObjectState state);
	void set_STATE(ObjectState state);

	ObjectState STATE;

	int move_time;

	//: RedLight images for each STATE.
	RE_Sprite* spr_object[1];

};

#endif
