/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_fireball
#define header_gameobject_fireball

#if _MSC_VER > 1000
#pragma once
#endif

#define PI 3.141593
#define FIREBALL_H_VELOCITY 10
#define FIREBALL_RAD_VELOCITY PI/16
#define FIREBALL_ROTATIONS 3

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "re_rect.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Fireball : public GameObject_Moving
{
//!Enums:
public:
	//: Different tile types in the game.
	enum ObjectState
	{
		active=0,
		waiting,
		inactive
	};

//!Construction:
public:
	//: Construct a player at position (x,y).
	GameObject_Fireball(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Fireball();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Fireball(world, object_element);
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

	void deploy(int new_x, int new_y, int state=0) {return;}

	virtual int get_total() {return 0;}
	virtual void set_total(int total) {return;}

	virtual bool is_dead(){return i_am_dead;}

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height(){return spr_object[0]->get_height();}

	void set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height);

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state, bool final = false);
	bool check_trigger();

	ObjectState STATE;

	RE_Rect* TriggerBox;

	int anim_pos;

	bool i_am_dead;

	int turn_time;

	float initial_angle;
	float angle;
	int center_x;
	int center_y;
	int radius;
	int direction; // > 0 clockwise, <= 0 counterclockwise
	int rotations;


	ObjectState previous_STATE;

	//: Object sprites for each STATE.
	RE_Sprite* spr_object[1];

	//:Sounds
	RE_SoundBuffer* sfx_moving;

	RE_SoundSession* sound;
};

#endif
