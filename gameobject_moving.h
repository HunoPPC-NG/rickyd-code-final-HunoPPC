/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_moving
#define header_gameobject_moving

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject.h"
#include "re_resourcemanager.h"
#include "re_rect.h"
#include "world.h"

#define STAIRS_DX 5 //default delta increase for stairs (add to left subtract from right)
#define GRAVITY 0.8   //global gravity
#define MAX_Y_VELOCITY 20 //clip y_velocity
// x<-------sprite width------>
// x<--dx--><--width--><--dx-->
// identical for y and height
// collisions are only checked for width and height regions of sprite
class CollisionBox
{
public:
	int dx;     //collision box delta x (delta to x property)
	int dy;	    //collision box delta y (delta to y property)
	int width;  //collision box width
	int height; //collision box height
	int stairs_dx; //delta increase for stairs
	int stairs_dy; //delta increase for stairs
};

class GameObject_Moving : public GameObject
{
//!Construction:
public:
	//: Construct object
	GameObject_Moving(World *world);

	//: Destructor.
	virtual ~GameObject_Moving();

//!Attributes:
public:
	virtual RE_Rect* get_collision_rect();
	virtual bool check_box_collision(RE_Rect *box);
	virtual bool check_point_collision(RE_Point *point);

	bool can_walk(); //check if we can walk without falling
	bool do_move_x(int dx, bool check_collision = true);
	bool do_move_y(int dy, bool check_collision = true);

	float x_velocity;
	float y_velocity;

	//collision box;
	CollisionBox CB;

	//collision box for complex objects
	CollisionBox CB1;
	CollisionBox CB2;
	CollisionBox CB3;

    RE_Rect *CB_rect;  //regular collision rect
    RE_Rect *CB1_rect; //extra collision rect for complex objects
    RE_Rect *CB2_rect; //extra collision rect for complex objects
    RE_Rect *CB3_rect; //extra collision rect for complex objects

    bool collision_left;
    bool collision_right;
    bool collision_top;
    bool collision_bottom;
    bool collision_stairs;
    bool collision_stairs_bottom;

protected:
	//: World object.
	World *world;

//!Operations:
protected:
	virtual void update_collision_box();

	//: Set animation speed of object. new_speed in frames per sec
	// speed is frame time in ms
	void set_speed(int new_speed) { speed = abs(1000/new_speed); }

	//: Set movement speed of object.
	void set_movement_speed(double new_speed) { movement_speed = new_speed; }

	//: Set object to move to this destination.
	void set_destination(int x, int y) { dest_x = x; dest_y = y; }

	//: Moves the object.
	bool move(int time_elapsed, bool check_collision = true);

//!Implementation:
protected:
	bool verify_move(int dx, int dy);

	bool can_stand(CollisionBox &stand_box);

	int dest_x, dest_y;

	int speed; //milisec (animation speed)
	double movement_speed; //pixels per milisec (movement speed - (game fps) = 40/1000 )
};

#endif
