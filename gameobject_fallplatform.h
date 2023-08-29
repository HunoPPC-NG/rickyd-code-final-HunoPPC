/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_fallplatform
#define header_gameobject_fallplatform

#if _MSC_VER > 1000
#pragma once
#endif

#define PLATFORM_TRIGGER_TIME 4000

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "re_rect.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Fallplatform : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active=0,
		crash,
		inactive
	};

//!Construction:
public:
	GameObject_Fallplatform(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	~GameObject_Fallplatform();
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
	std::string obj_name;

	void update_STATE();
	void set_STATE(ObjectState state, bool final = false);
	bool check_trigger();
	bool in_window(ObjectState state);

	bool triggered;

	ObjectState STATE;

	RE_Rect* TriggerBox;

	int anim_pos;

	bool i_am_dead;

	int turn_time;
	int move_time;
	int trigger_time;
	int trigger_timer;

	int trigger_state; // 0-only player can trigger; 1-kill player can also trigger

	ObjectState previous_STATE;

	//: Object sprites for each STATE.
	RE_Sprite* spr_object[2];

	//:Sounds
	RE_SoundBuffer* sfx_moving;

	RE_SoundSession* sound;
};

#endif
