/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_bomb
#define header_gameobject_bomb

#if _MSC_VER > 1000
#pragma once
#endif

#define MAX_BOMBS 6
#define ANIM_SPEED 4 //fps
#define BOMB_TIMER 6 //frames until explosion

#include "precomp.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "gameobject_moving.h"
#include "world.h"

class GameObject_Bomb : public GameObject_Moving
{
//!Enums:
public:
	//: object states.
	enum ObjectState
	{
		burning = 0,
		explosion,
		inactive
	};

//!Construction:
public:
	//: Construct a bomb object
	GameObject_Bomb(World *world);
	//: Destruct a bomb object
	~GameObject_Bomb();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Bomb(world);
	}
//!Attributes:
public:
	virtual void update_collision_box();

//!Operations:
public:
	//: Draw bomb onto graphic context.
	virtual void show(int view_x, int view_y);

	//: check time.
	virtual bool turn(int time_elapsed);

	//deploy a bomb
	void deploy(int bomb_x, int bomb_y, int state=0);

	//get number of available bombs
	virtual int get_total();
	//set number of available bombs
	virtual void set_total(int bombs);
	virtual bool is_dead(){return false;}

	virtual int get_width() {return spr_object[1]->get_width();}
	virtual int get_height(){return spr_object[1]->get_height();}

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state);

	int anim_pos;
	int timer;
	int deploy_state;

	ObjectState STATE;
	ObjectState previous_STATE;

	int turn_time;
	int slide;
	float friction;


	int bombs_available; //number of available bombs (out of max five)

	//: Bomb sprites for each STATE.
	RE_Sprite* spr_object[2];

	//: Sound effect played when bomb is created.
	RE_SoundBuffer* sfx_deploy;
	RE_SoundBuffer* sfx_explosion;

	RE_SoundSession* sound;
};

#endif
