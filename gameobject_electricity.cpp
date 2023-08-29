/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_electricity.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_Electricity construction:

GameObject_Electricity::GameObject_Electricity(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(kill_all);
	set_use_elevator(false);

	spr_object[moving] = new RE_Sprite(world->gc, "global/objects/electricity/moving/horizontal/sprite", world->global_resources);

	sfx_electricity = new RE_SoundBuffer("global/objects/electricity/sounds/shoot/sample", world->global_resources);
	sfx_electricity->set_volume(1.0f);

	sound = new RE_SoundSession();

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	anim_pos =  object_element->FirstChildElement("position")->IntAttribute("state");

	initial_x = current_position->x;
	initial_y = current_position->y;

	TriggerBox = NULL;
	SwitchBox = NULL;
	bounding_box = NULL;

	//check for trigger box
	tinyxml2::XMLElement* trigger_element = NULL;
	trigger_element = object_element->FirstChildElement("trigger_box");
	if(trigger_element)
	{
		int tb_x = trigger_element->IntAttribute("x");
		int tb_y = trigger_element->IntAttribute("y");
		int tb_width = trigger_element->IntAttribute("width");
		int tb_height = trigger_element->IntAttribute("height");

		set_trigger_box(tb_x, tb_y, tb_width, tb_height);
	}

	//check for switch box
	tinyxml2::XMLElement* switch_element = NULL;
	switch_element = object_element->FirstChildElement("switch_box");
	if(switch_element)
	{
		int tb_x = switch_element->IntAttribute("x");
		int tb_y = switch_element->IntAttribute("y");
		int tb_width = switch_element->IntAttribute("width");
		int tb_height = switch_element->IntAttribute("height");

		set_switch_box(tb_x, tb_y, tb_width, tb_height);
	}

	//check for bounding_box
	tinyxml2::XMLElement* bound_element = NULL;
	bound_element = object_element->FirstChildElement("bounding_box");
	if(bound_element)
	{
		int bb_x = bound_element->IntAttribute("x");
		int bb_y = bound_element->IntAttribute("y");
		int bb_width = bound_element->IntAttribute("width");
		int bb_height = bound_element->IntAttribute("height");

		set_bounding_box(bb_x, bb_y, bb_width, bb_height);
	}

	def_trigger_time = ELECTRICITY_TRIGGER_TIME;
	def_trigger_delay = 0;
	def_active_time = 0;
	moving_dir = 1;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("trigger_delay", &def_trigger_delay);
		param_element->QueryIntAttribute("active_time", &def_active_time);
		param_element->QueryIntAttribute("velocity", &moving_dir);
		param_element->QueryIntAttribute("def_trigger_time", &def_trigger_time);
	}

	set_STATE(moving);
	update_collision_box();
	if(TriggerBox)
		set_STATE(waiting);

	//anim_pos = 0;
	turn_time = 0;
	move_time = 0;
	trigger_time = 0;
	trigger_delay = def_trigger_delay;
	active_time = def_active_time;

	first_trigger = false;
	set_speed(4); //animation speed in frames per sec
}

GameObject_Electricity::~GameObject_Electricity()
{
	delete (spr_object[moving]);

	delete(sound);
	delete(sfx_electricity);

	if(TriggerBox)
		delete(TriggerBox);
	if(SwitchBox)
		delete(SwitchBox);
	if(bounding_box)
		delete(bounding_box);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Electricity attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Electricity operations:

void GameObject_Electricity::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

void GameObject_Electricity::set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	SwitchBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

void GameObject_Electricity::set_bounding_box(int bb_x, int bb_y, int bb_width, int bb_height)
{
	bounding_box = new RE_Rect(bb_x, bb_y - bb_height, bb_x + bb_width, bb_y);
}

bool GameObject_Electricity::check_switch()
{
	if(SwitchBox)
		return world->player->is_switch_trigger(SwitchBox);

	return false;
}

bool GameObject_Electricity::check_trigger()
{
	if(TriggerBox &&
		world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}

void GameObject_Electricity::update_STATE()
{
	if(check_switch())
		set_STATE(inactive);

	if(STATE == inactive)
		return;

	if(STATE == waiting && trigger_time <= 0)
	{
		if(check_trigger())
		{
			if(!first_trigger && trigger_delay > 0)
				trigger_delay -= turn_time;
			else
			{
				first_trigger = true;
				set_STATE(moving);
			}
		}
	}

	if(trigger_time > 0)
	{
		trigger_time -= turn_time;
	}

	if(STATE == waiting1 && trigger_time <= 0)
	{
		set_STATE(moving);
	}


	if(STATE != moving)
		return;


	//we are moving so play sfx
	if(!sound->is_playing())
		sound->play(sfx_electricity);

	if(bounding_box) //check for bounding box collision
	{
		if(current_position->x < bounding_box->Left() && moving_dir < 0)
		{
			moving_dir = 1; //go right
			set_STATE(waiting1);
		}
		else if(current_position->x + spr_object[moving]->get_width() > bounding_box->Right() && moving_dir > 0)
		{
			moving_dir = -1; //go left
			set_STATE(waiting1);
		}
	}

	if(STATE == moving)
	{
		x_velocity = moving_dir * ELECTRICITY_VELOCITY;

		if(def_active_time > 0)
		{
			if(active_time > 0)
				active_time -= turn_time;
			else
			{
				set_STATE(waiting);
			}
		}
	}
}

void GameObject_Electricity::set_STATE(ObjectState state)
{
	if(state == waiting)
	{
		set_type(kill_none);
		sound->stop();
		x_velocity = 0;
		current_position->x = initial_x;
		current_position->y = initial_y;
		active_time = def_active_time;
	}
	else if (state == waiting1)
	{
		set_type(kill_none);
		sound->stop();
		x_velocity = 0;
		trigger_time = def_trigger_time;
	}
	else
	{
		set_type(kill_all);
		if(state == moving)
		{
			trigger_time = def_trigger_time;
			x_velocity = moving_dir * ELECTRICITY_VELOCITY;
		}

	}

	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Electricity::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/electricity/moving/horizontal/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Electricity::turn(int time_elapsed)
{
	update_STATE();

	y_velocity = 0;

	if(STATE != moving)
		return false;

	turn_time = time_elapsed;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	return move(time_elapsed);
}

void GameObject_Electricity::show(int view_x, int view_y)
{
	if(STATE != moving)
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
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
