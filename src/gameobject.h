/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject
#define header_gameobject

#include "precomp.h"
#include "re_rect.h"
#include "re_point.h"

#if _MSC_VER > 1000
#pragma once
#endif

class GameObject
{
//:enums
public:
	//: object type.
	enum ObjectType
	{
		player = 0,
		kill_all,
		kill_player,
		kill_none,
		bonus_bullets,
		bonus_bombs,
		bonus_points,
		wall,
		platform,
		jump_platform,
		wall_elevator,
		platform_elevator,
		any //any of the above types
	};
//!Construction:
public:
	//:Constructor
	GameObject();
	//: Destructor.
	virtual ~GameObject();

//!Attributes:
public:
	//:Returns the collision rectangle
	virtual RE_Rect* get_collision_rect() = 0;

	//:checks for rectangle collision with object
	virtual bool check_box_collision(RE_Rect *box) = 0;

	//: checks for point collision with object
	virtual bool check_point_collision(RE_Point *point) = 0;

	void set_objects_iter(std::list<GameObject*>::iterator iter){current_object_iter = iter;}
	std::list<GameObject*>::iterator get_objects_iter(){return current_object_iter;}
	void next_objects_iter(){current_object_iter++;}

	void set_use_elevator(bool flag){ use_elevator = flag;}
	bool can_use_elevator(){return use_elevator;}
//!Operations:
public:
	//: Draw object on graphic context, where map is at (view_x, view_y).
	virtual void show(int view_x, int view_y) = 0;

	//: Moves the object around.
	virtual bool turn(int time_elapsed) = 0;

	//: deploys object
	virtual void deploy(int x, int y, int state = 0) = 0;

	virtual int get_total()=0;
	virtual void set_total(int total)=0;
	virtual bool is_dead()=0;

	virtual int get_width() = 0;
	virtual int get_height() = 0;

	virtual bool is_switch_trigger(RE_Rect* switch_button) {return false;}

	void set_object_collision(GameObject* obj_coll = NULL) {object_collision = obj_coll;}

	GameObject* get_object_collision() {return object_collision;}

	//:get object id
	Uint32 get_id() {return id;}

	//:get the object type.
	ObjectType get_type() {return TYPE;}

	void set_type(ObjectType type) {TYPE = type;}

	RE_Point* current_position; //GameObject position (sprite bottom left corner)

    bool on_climb;
    bool on_jump;
    bool on_elevator;
    bool can_move;
    bool foreground;

//!Implementation:
private:
	std::list<GameObject*>::iterator current_object_iter;
	ObjectType TYPE;
	GameObject* object_collision; //last object that collided with us or NULL if none
	Uint32 id;
	bool use_elevator; //can use elevator (def: true)
};

#endif
