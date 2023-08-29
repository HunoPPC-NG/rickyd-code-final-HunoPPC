/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_roller
#define header_gameobject_roller

#if _MSC_VER > 1000
#pragma once
#endif

#define ROLLER_VELOCITY 4

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Roller : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		moving_left = 0,
		moving_right,
		waiting,
		delayed,
		inactive
	};

//!Construction:
public:
	//: Construct a roller object
	GameObject_Roller(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	//: Destruct a roller object
	~GameObject_Roller();
//!Attributes:
public:
	virtual void update_collision_box();

//!Operations:
public:
	virtual void show(int view_x, int view_y);
	virtual bool turn(int time_elapsed);

	void deploy(int bomb_x, int bomb_y, int state=0) {return;}

	virtual int get_total() {return 0;}
	virtual void set_total(int total) {return;}

	virtual bool is_dead(){return false;}

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

//!Implementation:
private:
	std::string obj_name;
	void update_STATE();
	void set_state(int state);
	void set_STATE(ObjectState state);
	bool in_window(ObjectState state);
	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_bounding_box(int bb_x, int bb_y, int bb_width, int bb_height);
	bool check_trigger();

	RE_Rect* TriggerBox;  //trigger box
	RE_Rect* BoundingBox; //bounding box

	//-2 move right(repeat)
	//-1: move left(repeat)
	//1:move left(no repeat)
	//2:move right(no repeat)
	int initial_state;

	//bounce on wall or inactive/waiting
	bool bounce;
	// can be triggered by bullet
	bool bullet_trigger;
	// can be triggered by bomb
	bool bomb_trigger;
	//max falls before inactive/waiting
	int max_falls;
	int falls;
	bool falling;
	int try_change; //change after number of falls

	int delay_ind;

	int def_velocity;
	int def_trigger_delay;
	int def_trigger_time;

	int initial_x;
	int initial_y;

	int trigger_time;
	int trigger_delay;

	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;
	int move_time;

	//: Sprite images for each STATE.
	RE_Sprite* spr_object[4];

	//: Sound effect played when object is moving.
	RE_SoundBuffer* sfx_moving;
	RE_SoundSession* sound;

};

#endif
