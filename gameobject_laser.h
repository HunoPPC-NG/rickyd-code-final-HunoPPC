/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_laser
#define header_gameobject_laser

#if _MSC_VER > 1000
#pragma once
#endif

#define LASER_H_VELOCITY 10
#define LASER_V_VELOCITY 10
#define LASER_TRIGGER_TIME 1600 //ms

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Laser : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		moving_diagonal_left=0,
		moving_diagonal_right,
		splash,
		waiting,
		inactive
	};

//!Construction:
public:
	//: Construct a laser object
	GameObject_Laser(World *world, tinyxml2::XMLElement* object_element);
	//: Destruct a laser object
	~GameObject_Laser();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Laser(world, object_element);
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

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);
	void set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height);

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state);
	bool in_window(ObjectState state);

	int anim_pos;

	ObjectState STATE;
	ObjectState initial_STATE;

	ObjectState previous_STATE;

	int turn_time;
	int move_time;
	int trigger_time;
	int trigger_delay;
	bool first_trigger;

	// -1-diagonal left; 1-diagonal right;
	int laser_dir;
	int initial_x;
	int initial_y;
	int def_v_velocity;
	int def_h_velocity;
	int def_trigger_time;
	int def_trigger_delay;
	int deadly_explosion; //1- explosion kills; 0- doesn't kill

	// 1 - switch sets it off
	// 0  - switch sets it on
	int switch_state;
	bool laser_state;

	RE_Rect* TriggerBox; //trigger box
	RE_Rect* SwitchBox; //switch box

	bool check_trigger();
	bool check_switch();

	//: Laser images for each STATE.
	RE_Sprite* spr_object[4];

	//: Sound effects.
	RE_SoundBuffer* sfx_laser_moving;
	RE_SoundBuffer* sfx_laser_splash;
	RE_SoundSession* sound;

};

#endif
