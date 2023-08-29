/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_crane.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Crane construction:

GameObject_Crane::GameObject_Crane(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_STATE(waiting1);
	set_use_elevator(false);
	set_type(wall);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/crane/active/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/crane/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

	int type = 0;
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &type);

	switch(type)
	{
		case 0:
			direction = 0;
			def_type = wall_elevator;
			break;
		case 1:
			direction = 1;
			def_type = wall_elevator;
			break;
		case 2:
			direction = 0;
			def_type = kill_all;
			break;
		default:
			direction = 1;
			def_type = kill_all;
			break;
	}

	start_x = object_element->FirstChildElement("bounding_box")->IntAttribute("x");
	end_x = start_x + object_element->FirstChildElement("bounding_box")->IntAttribute("width");

	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);

	def_max_velocity = CRANE_MAX_VELOCITY;
	def_inc_velocity = CRANE_INC_VELOCITY;
	def_wait_time = CRANE_WAIT;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("max_velocity", &def_max_velocity);
		param_element->QueryIntAttribute("velocity_inc", &def_inc_velocity);
		param_element->QueryIntAttribute("wait_time", &def_wait_time);
	}

	update_collision_box();

	turn_time = 0;
	wait_time1 = def_wait_time;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Crane::~GameObject_Crane()
{
	delete(spr_object[active]);
	delete(TriggerBox);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Crane attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Crane operations:
void GameObject_Crane::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Crane::check_trigger()
{
	//check gameobject kill_all type collision (bullet, bomb, mummy)
	if(world->check_box_collision(kill_all, TriggerBox))
		return true;
	//check player collision
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}


void GameObject_Crane::update_STATE()
{
	if(STATE == waiting1) //waiting for trigger
	{
		if(check_trigger())
		{
			set_STATE(active);
		}
	}

	if(STATE == waiting2) //in start or end position
	{
		if(wait_time1 > 0)
		{
			wait_time1 -= turn_time;
		}
		else
		{
			set_STATE(active);
		}
	}


	if(STATE == active && direction == 0)
	{
		if(x_velocity < def_max_velocity)
			x_velocity += def_inc_velocity;
		else
			x_velocity = def_max_velocity;

		if(current_position->x >= end_x)
		{
			direction = 1;
			set_STATE(waiting2); //at end position (stop for some time)
		}
	}
	else if (STATE == active && direction == 1)
	{
		if(x_velocity >  -def_max_velocity)
			x_velocity -= def_inc_velocity;
		else
			x_velocity = -def_max_velocity;

		if(current_position->x <= start_x)
		{
			direction = 0;
			set_STATE(waiting2); //at start (stop for some time)
		}
	}

}

void GameObject_Crane::set_STATE(ObjectState state, bool final)
{
	if(state == waiting2)
	{
		wait_time1 = def_wait_time;
		x_velocity = 0;
		set_type(wall);
	}
	if(state == active)
	{
		set_type(def_type);
		x_velocity = 0;
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Crane::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/crane/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Crane::turn(int time_elapsed)
{
	update_STATE();

	turn_time = time_elapsed;

	bool ret = move(time_elapsed);

	anim_pos=0;
	return ret;
}

void GameObject_Crane::show(int view_x, int view_y)
{
	//only render if in window
	int scroll_delta = 0;
	if((world->scroll_y - scroll_delta) > current_position->y ||
	   (world->scroll_y + world->gc->get_height() + scroll_delta) < (current_position->y - spr_object[active]->get_height()))
		return;
	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		return;

	//int anim_length = spr_object[active]->get_frame_count();
	//if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[active]->set_frame(anim_pos);
	spr_object[active]->draw(
		 current_position->x  - view_x,
		(current_position->y - spr_object[active]->get_height() + 1) - view_y);
}
