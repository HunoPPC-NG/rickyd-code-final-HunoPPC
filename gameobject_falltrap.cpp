/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_falltrap.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Falltrap construction:

GameObject_Falltrap::GameObject_Falltrap(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_use_elevator(false);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/falltrap/active/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/falltrap/sounds/fall/sample", world->global_resources);
	sfx_moving->set_volume(1.0f);

	sound = new RE_SoundSession();

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	start_y = current_position->y;
	end_y = start_y + object_element->FirstChildElement("position")->IntAttribute("fall");

	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);
	set_STATE(waiting);

	def_time_to_fall = TRAP_TIME_TO_FALL;
	def_time_to_rearm = TRAP_TIME_TO_REARM;
	def_time_to_trigger = TRAP_TIME_TO_TRIGGER;
	def_velocity = TRAP_FALL_VELOCITY;
	wait_type = 0;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("trigger_time", &def_time_to_trigger);
		param_element->QueryIntAttribute("rearm_time", &def_time_to_rearm);
		param_element->QueryIntAttribute("fall_time", &def_time_to_fall);
		param_element->QueryIntAttribute("type", &wait_type);
		param_element->QueryIntAttribute("velocity", &def_velocity);
	}

	switch(wait_type)
	{
		case 1:
			set_type(wall);
			break;
		case 0:
		default:
			set_type(kill_none);
			break;
	}

	time_to_rearm = def_time_to_rearm;
	time_to_fall = def_time_to_fall;
	time_to_trigger = def_time_to_trigger;

	//check for a switch
	SwitchBox = NULL;
	tinyxml2::XMLElement* switch_element = NULL;
	switch_element = object_element->FirstChildElement("switch_box");
	if(switch_element)
	{
		int sb_x = switch_element->IntAttribute("x");
		int sb_y = switch_element->IntAttribute("y");
		int sb_width = switch_element->IntAttribute("width");
		int sb_height = switch_element->IntAttribute("height");

		set_switch_box(sb_x, sb_y, sb_width, sb_height);
		set_STATE(waiting_switch);
		set_type(kill_all);
		current_position->y = end_y; //start armed and at end position
		time_to_rearm = 0;
	}

	update_collision_box();

	anim_pos = 0;
	turn_time = 0;
	set_speed(4); //animation speed in frames per sec

}

GameObject_Falltrap::~GameObject_Falltrap()
{
	delete(spr_object[active]);

	delete(sfx_moving);
	delete(sound);
	delete(TriggerBox);
	if(SwitchBox)
		delete(SwitchBox);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Falltrap attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Falltrap operations:

void GameObject_Falltrap::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

void GameObject_Falltrap::set_switch_box(int sb_x, int sb_y, int sb_width, int sb_height)
{
	SwitchBox = new RE_Rect(sb_x, sb_y - sb_height, sb_x + sb_width, sb_y);
}

bool GameObject_Falltrap::check_trigger()
{
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}

bool GameObject_Falltrap::check_switch()
{
	bool ret = false;
	if(SwitchBox != NULL)
	{
		//check if player switched the trigger
		ret = world->player->is_switch_trigger(SwitchBox);
	}

	return ret;
}

void GameObject_Falltrap::update_STATE()
{

	if(STATE == waiting_switch && check_switch())
	{
		set_STATE(rearm);
	}

	if(STATE == waiting && check_trigger())
	{
		turn_time = 0;
		set_STATE(active);
	}
	else if(STATE == active && ((current_position->y) >= end_y))
	{
		y_velocity = 0;
		turn_time = 0;
		set_STATE(rearm);
		time_to_fall = def_time_to_fall;
	}
	else if(STATE == active && time_to_fall > 0)
	{
		time_to_fall -= turn_time;
	}
	else if(STATE == active && time_to_fall <= 0)
	{
		if(!sound->is_playing())
			sound->play(sfx_moving);
		set_type(kill_all);
		y_velocity = def_velocity;
	}
	else if(STATE == rearm && ((current_position->y) <= start_y))
	{
		set_STATE(rearmed);
		y_velocity = 0;
	}
	else if(STATE == rearm && time_to_rearm > 0)
	{
		time_to_rearm -= turn_time;
		if(wait_type == 1)
		{
			set_type(wall);
		}
	}
	else if(STATE == rearm && time_to_rearm <= 0)
	{
		if(!sound->is_playing())
			sound->play(sfx_moving);
		y_velocity = -(def_velocity/2);
		set_type(kill_all);
	}
	else if(STATE == rearmed && time_to_trigger > 0)
	{
		time_to_trigger -= turn_time;
	}
	else if(STATE == rearmed && time_to_trigger <= 0)
	{
		set_STATE(waiting);
		time_to_trigger = def_time_to_trigger;
	}

}

void GameObject_Falltrap::set_STATE(ObjectState state)
{
	if(state == rearmed)
	{
		time_to_rearm = def_time_to_rearm;
		switch(wait_type)
		{
			case 1:
				set_type(wall);
				break;
			case 0:
			default:
				set_type(kill_none);
				break;
		}
	}
	else if (state == waiting)
	{
		switch(wait_type)
		{
			case 1:
				set_type(wall);
				break;
			case 0:
			default:
				set_type(kill_none);
				break;
		}
	}


	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Falltrap::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/falltrap/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Falltrap::turn(int time_elapsed)
{
	update_STATE();
	//update_collision_box();

	turn_time = time_elapsed;

	return move(time_elapsed, false);
}

void GameObject_Falltrap::show(int view_x, int view_y)
{
	//only render if in window
	int scroll_delta = 0;
	if((world->scroll_y - scroll_delta) > current_position->y ||
	   (world->scroll_y + world->gc->get_height() + scroll_delta) < (current_position->y - spr_object[active]->get_height()))
		return;
	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		return;

	int anim_length = spr_object[active]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[active]->set_frame(anim_pos);
	spr_object[active]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[active]->get_height() +1) - view_y);
}

