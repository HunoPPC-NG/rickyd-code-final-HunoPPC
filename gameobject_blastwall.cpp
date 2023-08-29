/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_blastwall.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Blastwall construction:

GameObject_Blastwall::GameObject_Blastwall(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	obj_name = object_name;

	set_type(wall);
	set_use_elevator(false);
	set_STATE(active);

	spr_object[moving] = NULL;
	spr_object[blasting] = NULL;

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/active/wall/sprite", world->global_resources);
	if(world->global_resources->file_exists("global/objects/"+obj_name+"/active/moving/sprite"))
		spr_object[moving] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/active/moving/sprite", world->global_resources);
	if(world->global_resources->file_exists("global/objects/"+obj_name+"/active/blasting/sprite"))
		spr_object[blasting] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/active/blasting/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec
	int fgrd = 1; //foreground by default

	direction = 2;
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &direction);
	object_element->FirstChildElement("position")->QueryIntAttribute("foreground", &fgrd);

	if (fgrd > 0)
		foreground = true;

	TriggerBox = NULL;
	SwitchBox = NULL;

	//check for trigger box
	TriggerBox = NULL;
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

	//check for parameters
	int anim_velocity = 8;

	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("anim_velocity", &anim_velocity);
	}

	anim_pos = 0;
	turn_time = 0;
	set_speed(anim_velocity); //animation speed in frames per sec

	update_collision_box();

}

GameObject_Blastwall::~GameObject_Blastwall()
{
	delete(spr_object[active]);

	if(TriggerBox)
		delete(TriggerBox);
	if(SwitchBox)
		delete(SwitchBox);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Blastwall attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Blastwall operations:
void GameObject_Blastwall::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Blastwall::check_trigger()
{
	if( TriggerBox != NULL)
	{
		if( world->bomb->get_type() == kill_all )
			return(TriggerBox->is_overlapped(world->bomb->get_collision_rect()));
	}

	return false;
}

void GameObject_Blastwall::set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	SwitchBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Blastwall::check_switch()
{
	if(SwitchBox)
		return world->player->is_switch_trigger(SwitchBox);

	return false;
}

bool GameObject_Blastwall::in_window(ObjectState state)
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

void GameObject_Blastwall::update_STATE()
{
	if(STATE == inactive)
		return;

	x_velocity = 0;

	if(SwitchBox && STATE==active)
	{
		if(check_switch())
		{
			if(spr_object[moving] != NULL && direction != 0)
			{
				//slide and kill
				set_STATE(moving);
				set_type(kill_all);
				sound->play(sfx_moving);
			}
			else if(spr_object[blasting] != NULL)
			{
				//blast anim
				set_STATE(blasting);
			}
			else
			{
				//disabled
				set_STATE(inactive);
				set_type(kill_none);
			}
		}
		return;
	}

	if(TriggerBox && STATE==active)
	{
		if(check_trigger())
		{
			if(spr_object[moving] != NULL && direction != 0)
			{
				//slide and kill
				set_STATE(moving);
				set_type(kill_all);
				sound->play(sfx_moving);
			}
			else if(spr_object[blasting] != NULL)
			{
				//blast anim
				set_STATE(blasting);
			}
			else
			{
				//disabled
				set_STATE(inactive);
				set_type(kill_none);
			}
		}
		return;
	}

	//bomb explosion collides with wall type object
	if(STATE==active && world->bomb->get_type() == kill_all)
	{

		//check collision
		if(get_collision_rect()->is_overlapped(world->bomb->get_collision_rect()))
		{
			if(spr_object[moving] != NULL && direction != 0)
			{
				//slide and kill
				set_STATE(moving);
				set_type(kill_all);
				sound->play(sfx_moving);
			}
			else if(spr_object[blasting] != NULL)
			{
				//blast anim
				set_STATE(blasting);
			}
			else
			{
				//disabled
				set_STATE(inactive);
				set_type(kill_none);
			}

			return;
		}
	}

	if(STATE == moving)
	{
		if(!in_window(STATE))
		{
			set_STATE(inactive);
			return;
		}
		//move until we are off screen
		switch(direction)
		{
			case -2:
				y_velocity += BLOCK_VELOCITY;
				break;
			case -1:
				y_velocity -= BLOCK_VELOCITY;
				break;
			case 1:
				x_velocity += BLOCK_VELOCITY;
				break;
			default:
			case 2:
				x_velocity -= BLOCK_VELOCITY;
				break;
		}
	}
}

void GameObject_Blastwall::set_STATE(ObjectState state, bool final)
{
	if(state == inactive)
		set_type(kill_none);
	else if(state == blasting)
	{
		anim_pos = 0; //reset anim_pos
		set_type(kill_none);
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Blastwall::update_collision_box()
{
	if(STATE == inactive)
		return;

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Blastwall::turn(int time_elapsed)
{
	if(STATE == inactive)
		return false;

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(turn_time == 0) //rotate on turn start
		anim_pos++;


	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1)
	{
		anim_pos=0;
		if(STATE == blasting)
			set_STATE(inactive);
	}

	update_STATE();

	bool ret = move(time_elapsed, false);

	return ret;
}

void GameObject_Blastwall::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	//only render if in window
	if(!in_window(STATE))
		return;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		 current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
