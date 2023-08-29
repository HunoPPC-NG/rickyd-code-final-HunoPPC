/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_fallplatform.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Caveman1 construction:

GameObject_Fallplatform::GameObject_Fallplatform(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	obj_name = object_name;

	spr_object[crash] = NULL;
	set_STATE(active);
	triggered = false;

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/active/anim/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/crash/anim/sprite"))
		spr_object[crash] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/crash/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	TriggerBox = NULL;

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

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

	trigger_timer = PLATFORM_TRIGGER_TIME;
	trigger_state = 0; //only player can trigger

	int type = 0;
	int anim_velocity = 8;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("type", &type);
		param_element->QueryIntAttribute("trigger", &trigger_state);
		param_element->QueryIntAttribute("trigger_time", &trigger_timer);
		param_element->QueryIntAttribute("anim_velocity", &anim_velocity);
	}

	if(type == 0)
		set_type(wall);     //wall
	else
		set_type(platform); //platform

	trigger_time = trigger_timer;

	update_collision_box();

	turn_time = 0;
	move_time = 0;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Fallplatform::~GameObject_Fallplatform()
{
	delete(spr_object[active]);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Fallplatform attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Fallplatform operations:
void GameObject_Fallplatform::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Fallplatform::check_trigger()
{
	if( TriggerBox != NULL)
	{
		switch(trigger_state)
		{
			case 1:
				if( world->player->get_type() != kill_none &&
					TriggerBox->is_overlapped(world->player->get_collision_rect()) )
					return true;
				if( world->check_box_collision(kill_player, TriggerBox, this) != NULL )
					return true;
				break;

			case 0:
			default:
				if( world->player->get_type() != kill_none &&
					TriggerBox->is_overlapped(world->player->get_collision_rect()))
					return true;
				break;
		}
	}
	else
		return true;

	return false;
}

bool GameObject_Fallplatform::in_window(ObjectState state)
{
	if(!spr_object[state])
		return false;

	int scroll_delta = 0;
	if((world->scroll_y - scroll_delta) > current_position->y ||
	   (world->scroll_y + world->gc->get_height() + scroll_delta) < (current_position->y - spr_object[state]->get_height()))
		return false;
	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		return false;

	return true;
}

void GameObject_Fallplatform::update_STATE()
{
	if(STATE == inactive)
		return;

	if(STATE == active)
	{
		if(trigger_time >= trigger_timer)
		{
			if(check_trigger())
			{
				trigger_time -= turn_time;
				triggered = true;
			}
		}

		if(triggered)
		{
			if(trigger_time > 0)
				trigger_time -= turn_time;
			else if(spr_object[crash] != NULL)
				set_STATE(crash);
			else
				set_STATE(inactive);
		}
	}

	if(STATE == crash)
	{
		if(!collision_bottom && in_window(STATE))
			y_velocity += GRAVITY;
		else
			set_STATE(inactive);
	}
}

void GameObject_Fallplatform::set_STATE(ObjectState state, bool final)
{
	if(state == inactive || state == crash)
	{
		set_type(kill_none);
		anim_pos=0;//reset
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;

	update_collision_box();
}

void GameObject_Fallplatform::update_collision_box()
{
	std::string action = "";
	switch(STATE)
	{
		case crash:
			action = "crash";
			break;

		case active:
		default:
			action = "active";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Fallplatform::turn(int time_elapsed)
{
	update_STATE();

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	turn_time = time_elapsed;

	bool ret = move(time_elapsed);

	return ret;
}

void GameObject_Fallplatform::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	//only render if in window
	if(!in_window(STATE))
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[STATE]->set_frame(anim_pos);

	spr_object[STATE]->draw(
		 current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() + 1) - view_y);
}
