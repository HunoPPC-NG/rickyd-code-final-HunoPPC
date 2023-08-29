/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_saw.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Saw construction:

GameObject_Saw::GameObject_Saw(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(kill_all);

	spr_object[up] = new RE_Sprite(world->gc, "global/objects/saw/up/anim/sprite", world->global_resources);
	spr_object[down] = new RE_Sprite(world->gc, "global/objects/saw/down/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/saw/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;
	turn_time = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

	int type = 0;
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &type);

	switch(type)
	{
		case 0:
			saw_dir = left;
			set_STATE(up);
			break;
		case 1:
			saw_dir = right;
			set_STATE(up);
			break;
		case 2:
			saw_dir = left;
			set_STATE(down);
			break;
		default:
			saw_dir = right;
			set_STATE(down);
			break;
	}

	def_velocity = SAW_VELOCITY;
	def_wait_time = SAW_WAIT_TIME;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("velocity", &def_velocity);
		param_element->QueryIntAttribute("wait_time", &def_wait_time);
	}
	wait_time = 0; //no wait time at start

	int bb_x = object_element->FirstChildElement("bounding_box")->IntAttribute("x");
	int bb_y = object_element->FirstChildElement("bounding_box")->IntAttribute("y");
	int bb_width = object_element->FirstChildElement("bounding_box")->IntAttribute("width");
	int bb_height = object_element->FirstChildElement("bounding_box")->IntAttribute("height");

	bounding_box = new RE_Rect(bb_x, bb_y - bb_height, bb_x + bb_width, bb_y);

	update_collision_box();
}

GameObject_Saw::~GameObject_Saw()
{
	delete(spr_object[up]);
	delete(spr_object[down]);

	delete(bounding_box);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Saw attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Saw operations:

void GameObject_Saw::update_STATE()
{
	if(STATE == inactive)
		return;

	if(saw_dir == left &&
	  ((current_position->x < bounding_box->Left()) || collision_left))
	{
		if(wait_time <= 0)
			wait_time = def_wait_time;
		saw_dir = right;
	}
	else if(saw_dir == right &&
	  ((current_position->x + spr_object[STATE]->get_width() > bounding_box->Right()) || collision_right))
	{
		if(wait_time <= 0)
			wait_time = def_wait_time;
		saw_dir = left;
	}

	if(wait_time <= 0)
	{
		x_velocity = def_velocity * saw_dir;
		set_speed(5); //animation speed in frames per sec
	}
	else
	{
		x_velocity = 0;
		set_speed(20); //animation speed in frames per sec
	}
	y_velocity = 0;
}

void GameObject_Saw::set_STATE(ObjectState state, bool final)
{
	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Saw::update_collision_box()
{
	if(STATE == inactive)
		return;

	std::string action;

	switch(STATE)
	{
		case up:
			action = "up";
			break;
		case down:
			action = "down";
			break;
	}
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/saw/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Saw::turn(int time_elapsed)
{
	update_STATE();

	if(wait_time > 0)
		wait_time -= time_elapsed;

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	bool ret = move(time_elapsed);

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	return ret;
}

void GameObject_Saw::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	//only render if in window
	int scroll_delta = 0;
	if((world->scroll_y - scroll_delta) > current_position->y ||
	   (world->scroll_y + world->gc->get_height() + scroll_delta) < (current_position->y - spr_object[STATE]->get_height()))
		return;
	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;
	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		 current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() + 1) - view_y);
}
