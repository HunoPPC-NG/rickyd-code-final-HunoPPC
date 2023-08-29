/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_climber
#define header_gameobject_climber

#if _MSC_VER > 1000
#pragma once
#endif

#define CLIMBER_VELOCITY 4

#define CLIMBER_FREEZE_TIME 140

#define CLIMBER_WAIT_CHANGE_DIR_MAX 2500
#define CLIMBER_WAIT_CHANGE_DIR_MIN 600

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Climber : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		walk_left=0,
		walk_right,
		climb,
		dead,
		inactive
	};

//!Construction:
public:
	GameObject_Climber(World *world, tinyxml2::XMLElement* object_element, const std::string object_name);
	~GameObject_Climber();
//!Attributes:
public:
	virtual void update_collision_box();

//!Operations:
public:
	virtual void show(int view_x, int view_y);
	virtual bool turn(int time_elapsed);

	void deploy(int new_x, int new_y, int state=0);

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
	void set_state(int state);
	bool play_sound();
	bool do_random(int chance = 50);
	bool check_trigger();
	bool in_window(ObjectState state);

	int get_change_dir_time();

	int freeze_time;

	int anim_pos;

	bool i_am_dead;
	bool follow_player;
    bool follow_vertical;
    bool in_pursuit;
    int def_wait_change_dir_max;
    int def_wait_change_dir;
    int def_wait_change_dir_min;
	int wait_change_dir;
	int follow_delta_x;
	int follow_delta_y;
	int check_fall; //0 - no check; 1- check if move causes fall (invert movement)
	int initial_scroll_x;

    float stored_velocity;

	ObjectState STATE;
	void set_STATE(ObjectState state, bool final = false);

	int turn_time;
	int move_time;

	RE_Rect* bounding_box;
	RE_Rect* TriggerBox;

	std::vector<int> stairs_x; // x for stairs that may be used

	ObjectState previous_STATE;

	//: Object sprites for each STATE.
	RE_Sprite* spr_object[4];

	//:Sounds
	RE_SoundBuffer* sfx_dead;

	RE_SoundSession* sound;
};

#endif