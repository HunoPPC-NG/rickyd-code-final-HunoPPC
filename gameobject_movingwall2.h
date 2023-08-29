/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_movingwall2
#define header_gameobject_movingwall2

#if _MSC_VER > 1000
#pragma once
#endif

#define MOVINGBLOCK2_H_VELOCITY 5
#define MOVINGBLOCK2_V_VELOCITY 5
#define MOVINGBLOCK2_WAIT1 12000
#define MOVINGBLOCK2_TRIGGER_TIME 600

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "re_rect.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Movingwall2 : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active=0,
	};

//!Construction:
public:
	GameObject_Movingwall2(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Movingwall2();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element) 
	{ 
		return new GameObject_Movingwall2(world, object_element); 
	}
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
	void update_STATE();
	void set_STATE(ObjectState state, bool final = false);
	bool check_trigger();
	
	ObjectState STATE;
	
	RE_Rect* TriggerBox;
	
	int anim_pos;

	bool i_am_dead;
	
	int direction; //0 left, 1 right
	
	int turn_time;
	int trigger_time;
	int wait_time1;

	int start_x;
	int start_y;
	int end_x;
	int end_y;
	
	bool check_wall;
	
	ObjectState previous_STATE;
	
	//: Object sprites for each STATE.
	RE_Sprite* spr_object[1];
	
	//:Sounds
	RE_SoundBuffer* sfx_moving;
	
	RE_SoundSession* sound;
};

#endif
