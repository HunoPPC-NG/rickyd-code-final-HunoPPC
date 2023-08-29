/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_vprojectile
#define header_gameobject_vprojectile

#if _MSC_VER > 1000
#pragma once
#endif

#define VPROJ_VELOCITY 15
#define VPROJ_TRIGGER_TIME 1600 //ms

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Vprojectile : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		moving_up = 0,
		moving_down,
		splash_up,
		splash_down,
		waiting,
		inactive
	};

//!Construction:
public:
	//: Construct a vprojectile object
	GameObject_Vprojectile(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	//: Destruct a vprojectile object
	~GameObject_Vprojectile();
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
	std::string obj_name;
	void update_STATE();
	void set_STATE(ObjectState state);
	bool in_window(ObjectState state);

	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;
	int move_time;
	int trigger_time;
	int trigger_delay;
	bool triggered;

	int proj_dir; // -2: move down only once
	              // -1: move down multiple times
	              //  1: move up multiple times
	              //  2: move up only once
	int initial_x;
	int initial_y;
	int def_velocity;
	int def_trigger_time;
	int def_trigger_delay;
	int wall_collision;
	int deadly_splash; //1- splash kills; 0- doesn't kill
	// 1 - switch sets it off
	// 0  - switch sets it on
	int switch_state;
	//0 - only player can trigger
	//1 - player and kill_player can trigger
	int trigger_state;
	bool is_on;

	RE_Rect* TriggerBox; //trigger box
	RE_Rect* SwitchBox; //switch box

	bool check_trigger();
	bool check_switch();

	//: images for each STATE.
	RE_Sprite* spr_object[5];

	//: Sound effect played
	RE_SoundBuffer* sfx_moving;
	RE_SoundBuffer* sfx_splash;
	RE_SoundSession* sound;

};

#endif
