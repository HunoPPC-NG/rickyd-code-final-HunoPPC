/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_jewel
#define header_gameobject_jewel

#if _MSC_VER > 1000
#pragma once
#endif

#define JEWEL_BONUS 1000
#define JEWEL_SCORE_TIME 1000

#include "precomp.h"
#include "gameobject_moving.h"
#include "re_font.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Jewel : public GameObject_Moving
{
//!Enums:
public:
	enum ObjectState
	{
		active = 0,
		captured,
		inactive
	};

//!Construction:
public:
	//: Construct a jewel object
	GameObject_Jewel(World *world,  tinyxml2::XMLElement* object_element);
	//: Destruct a jewel object
	~GameObject_Jewel();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element) 
	{ 
		return new GameObject_Jewel(world, object_element); 
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

//!Implementation:
private:
	void update_STATE();
	void set_STATE(ObjectState state);
	
	int bonus_value;
	
	int anim_pos;

	ObjectState STATE;
	ObjectState previous_STATE;
	
	int turn_time;
	int score_time;
	
	//: Jewel images for each STATE.
	RE_Sprite* spr_object[1];
	RE_Font* score_font;
	
	//: Sound effect played when bonus is collected.
	RE_SoundBuffer* sfx_jewel;
	
	RE_SoundSession* sound;

};

#endif
