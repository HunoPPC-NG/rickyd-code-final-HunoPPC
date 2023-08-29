/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "gameobject_traptrigger.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Spearstrap construction:

GameObject_Traptrigger::GameObject_Traptrigger(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	obj_name = object_name;
	set_use_elevator(false);

	spr_object[active_left] = NULL;
	spr_object[active_right] = NULL;
	spr_object[active_up] = NULL;
	spr_object[active_down] = NULL;

	set_type(kill_all);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/horizontal/left/sprite"))
		spr_object[active_left] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/horizontal/left/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/horizontal/right/sprite"))
		spr_object[active_right] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/horizontal/right/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/vertical/up/sprite"))
		spr_object[active_up] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/vertical/up/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/vertical/down/sprite"))
		spr_object[active_down] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/vertical/down/sprite", world->global_resources);


	sfx_traptrigger = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/active/sample", world->global_resources);
	sfx_traptrigger->set_volume(1.0f);

	sound = new RE_SoundSession();

	initial_state = 0;
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &initial_state);
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

	if(initial_state == 0 && !spr_object[active_up])
	{
		std::cerr<< "GameObject_Traptrigger: (FATAL) direction is set to up but no matching sprite found\n";
		exit(-1);
	}

	if(initial_state == 1 && !spr_object[active_down])
	{
		std::cerr<< "GameObject_Traptrigger: (FATAL) direction is set to down but no matching sprite found\n";
		exit(-1);
	}

	if(initial_state == 2 && !spr_object[active_left])
	{
		std::cerr<< "GameObject_Traptrigger: (FATAL) direction is set to left but no matching sprite found\n";
		exit(-1);
	}

	if(initial_state == 3 && !spr_object[active_right])
	{
		std::cerr<< "GameObject_Traptrigger: (FATAL) direction is set to right but no matching sprite found\n";
		exit(-1);
	}

	if(initial_state > 3 || initial_state < 0)
		initial_state = 4; //waiting

	initial_x = current_position->x;
	initial_y = current_position->y;

	TriggerBox = NULL;
	TriggerBox2 = NULL;
	SwitchBox = NULL;

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

	//check for second trigger box
	trigger_element = NULL;
	trigger_element = object_element->FirstChildElement("trigger_box_2");
	if(trigger_element)
	{
		int tb_x = trigger_element->IntAttribute("x");
		int tb_y = trigger_element->IntAttribute("y");
		int tb_width = trigger_element->IntAttribute("width");
		int tb_height = trigger_element->IntAttribute("height");

		set_trigger_box_2(tb_x, tb_y, tb_width, tb_height);
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
	def_rearm_time = -1; //no rearm
	switch_state  = 1;
	trigger_state = 0;

	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("switch", &switch_state);
		param_element->QueryIntAttribute("trigger", &trigger_state);
		param_element->QueryIntAttribute("anim_velocity", &anim_velocity);
		param_element->QueryIntAttribute("rearm_time", &def_rearm_time);
	}

	time_to_rearm = def_rearm_time;

	is_on = false;

	if(switch_state > 0)
		is_on = true;

	if(TriggerBox == NULL || trigger_state > 0) //active from start
	{
		set_state(initial_state);
		is_on = true;
	}
	else
		set_STATE(waiting);

	update_collision_box();

	anim_pos = 0;
	turn_time = 0;
	set_speed(anim_velocity); //animation speed in frames per sec

}

GameObject_Traptrigger::~GameObject_Traptrigger()
{
	if(spr_object[active_up])
		delete(spr_object[active_up]);
	if(spr_object[active_down])
		delete(spr_object[active_down]);
	if(spr_object[active_left])
		delete(spr_object[active_left]);
	if(spr_object[active_right])
		delete(spr_object[active_right]);

	if(TriggerBox)
		delete(TriggerBox);
	if(TriggerBox2)
		delete(TriggerBox2);
	if(SwitchBox)
		delete(SwitchBox);

	delete(sfx_traptrigger);
	delete(sound);

}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Traptrigger attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Traptrigger operations:

void GameObject_Traptrigger::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

void GameObject_Traptrigger::set_trigger_box_2(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox2 = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Traptrigger::check_trigger()
{
	if( TriggerBox != NULL)
	{
		//check trigger box collision with bullet
		if(
			trigger_state >=0 &&
			(
		      world->bullet->get_type() == kill_all &&
		      TriggerBox->is_overlapped(world->bullet->get_collision_rect())
		    )
		  )
		  return true;

		//we have a second trigger box : check it
		if(TriggerBox2 != NULL)
		{
			if(
				trigger_state >=0 &&
				(
				  world->bullet->get_type() == kill_all &&
				  TriggerBox2->is_overlapped(world->bullet->get_collision_rect())
				)
			)
			return true;
		}

		if(trigger_state < 2 && world->player->get_type() != kill_none)
			return(TriggerBox->is_overlapped(world->player->get_collision_rect()));
	}
	else if(trigger_state > 0 &&
		world->bomb->get_type() == kill_all &&
		get_collision_rect()->is_overlapped(world->bomb->get_collision_rect()))
	{
		//no trigger box and it's active check for bomb blast to turn off
		return true;
	}
	else if (trigger_state <= 0)
		return true;

	return false;
}

void GameObject_Traptrigger::set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	SwitchBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Traptrigger::check_switch()
{
	if(!SwitchBox)
		return false;
	//check switch box collision with bullet
	if(	switch_state >= 2 &&
	    ( world->bullet->get_type() == kill_all &&
		  SwitchBox->is_overlapped(world->bullet->get_collision_rect())
		))
		return true;

	if(switch_state < 2)
		return world->player->is_switch_trigger(SwitchBox);

	return false;
}

bool GameObject_Traptrigger::in_window(ObjectState state)
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

void GameObject_Traptrigger::update_STATE()
{
	if( switch_state > 0 && check_switch()) //turn trap off
		is_on = false;
	else if(switch_state == 0 && check_switch()) //turn trap on
		is_on = true;

	if(is_on)
	{
		if(STATE == waiting && trigger_state <=0 && check_trigger())
		{
			if(!sound->is_playing())
				sound->play(sfx_traptrigger);

			set_state(initial_state);
		}
		else if(STATE == waiting && time_to_rearm > 0)
		{
			time_to_rearm -= turn_time;
		}
		else if(STATE == waiting && def_rearm_time > 0 && time_to_rearm < 0)
		{
			set_state(initial_state);
			time_to_rearm = def_rearm_time;
			if(!sound->is_playing())
				sound->play(sfx_traptrigger);
		}
		else if(STATE != waiting && trigger_state > 0 && check_trigger())
		{
			set_STATE(waiting);
			if(TriggerBox == NULL)
				is_on = false;
		}
	}
	else
	{
		set_STATE(waiting);

		if(switch_state > 0) //is on by default (rearm if def_rearm_time)
		{
			if(time_to_rearm > 0)
			{
				time_to_rearm -= turn_time;
			}
			else if(def_rearm_time > 0 && time_to_rearm < 0)
			{
				set_state(initial_state);
				is_on = true;
				time_to_rearm = def_rearm_time;
				if(!sound->is_playing())
					sound->play(sfx_traptrigger);
			}
		}

	}

}

void GameObject_Traptrigger::set_state(int state)
{
	switch(state)
	{
		case 1:
			set_STATE(active_down);
			break;
		case 2:
			set_STATE(active_left);
			break;
		case 3:
			set_STATE(active_right);
			break;
		case 0:
		default:
			set_STATE(active_up);
			break;
	}
}

void GameObject_Traptrigger::set_STATE(ObjectState state)
{
	if(state == waiting)
		set_type(kill_none);
	else
		set_type(kill_all);

	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Traptrigger::update_collision_box()
{
	if(STATE == inactive)
		return;

	std::string action;

	switch(STATE)
	{
		case active_right:
		case active_left:
			action = "horizontal";
			break;
		case active_up:
		case active_down:
		default:
			action = "vertical";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Traptrigger::turn(int time_elapsed)
{
	turn_time = time_elapsed;

	update_STATE();
	//update_collision_box();

	return move(time_elapsed, false);
}

void GameObject_Traptrigger::show(int view_x, int view_y)
{
	if( STATE == waiting ||  STATE == inactive)
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

