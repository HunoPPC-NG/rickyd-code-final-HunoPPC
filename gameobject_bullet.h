/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_bullet
#define header_gameobject_bullet

#if _MSC_VER > 1000
#pragma once
#endif

#define MAX_BULLETS 6
#define BULLET_VELOCITY 15
#define BULLET_TRIGGER_TIME 100

#include "precomp.h"
#include "gameobject_moving.h"
#include "world.h"
#include "re_resourcemanager.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "re_sprite.h"

class GameObject_Bullet : public GameObject_Moving
{
//!Enums:
public:
	//: Different tile types in the game.
	enum ObjectState
	{
		moving_left = 0,
		moving_right,
		overbike_left,
		overbike_right,
		inactive
	};

//!Construction:
public:
	//: Construct a bullet object
	GameObject_Bullet(World *world);
	//: Destruct a bullet object
	~GameObject_Bullet();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Bullet(world);
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

	//shoot a bullet:
	//  dir = 0: left
	//  dir = 1: right
	//  dir = 2: overdrive left
	//  dir = 3: overdrive right
	virtual void deploy(int start_x, int start_y, int dir=0);

	virtual bool is_dead(){return false;}

	//get number of available bullets
	virtual int get_total();
	//set number of available bullets
	virtual void set_total(int bullets);

	virtual int get_width() {return spr_object[0]->get_width();}
	virtual int get_height() {return spr_object[0]->get_height();}

	int get_dir_width(int dir=0);
	int get_dir_height(int dir=0);


//!Implementation:
private:
	bool in_window(ObjectState state);
	void update_STATE();
	void set_STATE(ObjectState state);

	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;
	int trigger_time;

	int bullets_available; //number of available bullets (out of max five)
	bool i_am_dead;

	//: Bullet images for each STATE.
	RE_Sprite* spr_object[4];

	RE_SoundBuffer* sfx_shoot;

	RE_SoundSession* sound;
};

#endif
