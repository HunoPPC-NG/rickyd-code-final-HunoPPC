/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_laser.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_Laser construction:

GameObject_Laser::GameObject_Laser(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(kill_none);
	set_use_elevator(false);

	spr_object[moving_diagonal_left] = new RE_Sprite(world->gc, "global/objects/laser/moving/diagonal/left/sprite", world->global_resources);
	spr_object[moving_diagonal_right] = new RE_Sprite(world->gc, "global/objects/laser/moving/diagonal/right/sprite", world->global_resources);
	spr_object[splash] = new RE_Sprite(world->gc, "global/objects/laser/splash/sprite", world->global_resources);

	sfx_laser_moving = new RE_SoundBuffer("global/objects/laser/sounds/moving/sample", world->global_resources);
	sfx_laser_splash = new RE_SoundBuffer("global/objects/laser/sounds/splash/sample", world->global_resources);
	sfx_laser_moving->set_volume(1.0f);
	sfx_laser_splash->set_volume(1.0f);

	sound = new RE_SoundSession();

	laser_dir = -1; //diagonal left by default
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &laser_dir);

	switch(laser_dir)
	{
		case -1:
			initial_STATE = moving_diagonal_left;
			break;
		case 1:
		default:
			initial_STATE = moving_diagonal_right;
			break;
	}

	initial_x = current_position->x;
	initial_y = current_position->y;

	TriggerBox = NULL;
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

	def_h_velocity = LASER_H_VELOCITY;
	def_v_velocity = LASER_V_VELOCITY;

	def_trigger_time = LASER_TRIGGER_TIME;
	def_trigger_delay = 0;
	switch_state = 1;
	deadly_explosion = false;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("switch", &switch_state);
		param_element->QueryIntAttribute("horizontal_velocity", &def_h_velocity);
		param_element->QueryIntAttribute("vertical_velocity", &def_v_velocity);
		param_element->QueryIntAttribute("trigger_time", &def_trigger_time);
		param_element->QueryIntAttribute("trigger_delay", &def_trigger_delay);
		param_element->QueryIntAttribute("deadly_explosion", &deadly_explosion);
	}

	if(switch_state > 0)
		laser_state = true;
	else
		laser_state = false;


	set_STATE(waiting);
	update_collision_box();

	anim_pos  = 0;
	turn_time = 0;
	move_time = 0;
	trigger_time = 0;
	trigger_delay = def_trigger_delay;
	first_trigger = false;
	set_speed(16); //animation speed in frames per sec
}

GameObject_Laser::~GameObject_Laser()
{
	delete (spr_object[moving_diagonal_right]);
	delete (spr_object[moving_diagonal_left]);
	delete (spr_object[splash]);

	delete(sound);
	delete(sfx_laser_moving);
	delete(sfx_laser_splash);

	if(TriggerBox)
		delete(TriggerBox);
	if(SwitchBox)
		delete(SwitchBox);

}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Laser attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Laser operations:

void GameObject_Laser::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

void GameObject_Laser::set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	SwitchBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Laser::check_switch()
{
	if(SwitchBox)
		return world->player->is_switch_trigger(SwitchBox);

	return false;
}

bool GameObject_Laser::check_trigger()
{
	if( TriggerBox != NULL &&
		world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));
	else if (!TriggerBox)
		return true;

	return false;
}

bool GameObject_Laser::in_window(ObjectState state)
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

void GameObject_Laser::update_STATE()
{
	if( switch_state > 0 && check_switch()) //turn laser off
		laser_state = false;
	else if(switch_state == 0 && check_switch()) //turn laser on
		laser_state = true;

	if(trigger_time > 0)
	{
		trigger_time -= turn_time;
	}

	if(STATE == waiting && trigger_time <= 0)
	{
		if(laser_state) //laser is on
		{
			if(check_trigger())
			{
				if(!first_trigger && trigger_delay > 0)
					trigger_delay -= turn_time;
				else
				{
					first_trigger = true;
					set_STATE(initial_STATE);
				}
			}
		}
	}

	if(STATE == waiting ||
	   STATE == inactive)
		return;

	//check for wall hit
	if( STATE != splash &&
		( collision_left ||
		  collision_right ||
		  //current_position->x < world->scroll_x ||
		  //current_position->x > world->scroll_x + world->gc->get_width() - spr_object[initial_STATE]->get_width() ||
		  collision_bottom ||
		  collision_top //||
		  //current_position->y < world->scroll_y - spr_object[initial_STATE]->get_height() ||
		 //current_position->y > world->scroll_y + world->gc->get_height()
		 ))
	{
		set_STATE(splash);
	}

	switch(STATE)
	{
		case moving_diagonal_left:
			x_velocity = -def_h_velocity;
			y_velocity = def_v_velocity;
			break;

		case moving_diagonal_right:
			x_velocity = def_h_velocity;
			y_velocity = def_v_velocity;
			break;

		case splash:
			x_velocity = 0;
			y_velocity = 0;
			break;
	}
}

void GameObject_Laser::set_STATE(ObjectState state)
{
	switch(state)
	{
		case waiting:
			set_type(kill_none);
			sound->stop();
			x_velocity = 0;
			y_velocity = 0;
			current_position->x = initial_x;
			current_position->y = initial_y;
			anim_pos = 0;
			break;
		case splash:
			anim_pos = 0;
			move_time = 0;
			if(deadly_explosion > 0)
				set_type(kill_all);
			else
				set_type(kill_none);
			if(in_window(state))
			{
				sound->stop();
				sound->play(sfx_laser_splash);
			}
			collision_left = false;
			collision_right = false;
			collision_bottom = false;
			collision_top = false;
			//center blast
			switch(initial_STATE)
			{
				case moving_diagonal_left:
					current_position->x -= spr_object[splash]->get_width()/2;
					current_position->y += spr_object[splash]->get_height()/2;
					break;

				case moving_diagonal_right:
					current_position->x += (spr_object[initial_STATE]->get_width() - spr_object[splash]->get_width()/2);
					current_position->y += spr_object[splash]->get_height()/2;
					break;
			}
			break;
		default:
			set_type(kill_all);
			anim_pos = 0;
			trigger_time = def_trigger_time;
			if(!sound->is_playing()	&& in_window(state) )
				sound->play(sfx_laser_moving);
			break;
	}

	previous_STATE = STATE;
	STATE = state;

	update_collision_box();
}


void GameObject_Laser::update_collision_box()
{
	std::string action = "";
	switch(STATE)
	{
		case moving_diagonal_left:
			action = "moving/diagonal/left";
			break;
		case moving_diagonal_right:
			action = "moving/diagonal/right";
			break;
		case splash:
		default:
			action = "splash";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/laser/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");

    tinyxml2::XMLElement* cb1_element = world->global_resources->getElement("global/objects/laser/"+action+"/collision_box1", false);
    if(cb1_element)
    {
		CB1.width = cb1_element->IntAttribute("width");
		CB1.height = cb1_element->IntAttribute("height");
		CB1.dx = cb1_element->IntAttribute("dx"); //delta to x
		CB1.dy = cb1_element->IntAttribute("dy"); //delta to y
	}
	else
		CB1.width = 0;

    tinyxml2::XMLElement* cb2_element = world->global_resources->getElement("global/objects/laser/"+action+"/collision_box2", false);
    if(cb2_element)
    {
		CB2.width = cb2_element->IntAttribute("width");
		CB2.height = cb2_element->IntAttribute("height");
		CB2.dx = cb2_element->IntAttribute("dx"); //delta to x
		CB2.dy = cb2_element->IntAttribute("dy"); //delta to y
	}
	else
		CB2.width = 0;

	tinyxml2::XMLElement* cb3_element = world->global_resources->getElement("global/objects/laser/"+action+"/collision_box3", false);
    if(cb3_element)
    {
		CB3.width = cb3_element->IntAttribute("width");
		CB3.height = cb3_element->IntAttribute("height");
		CB3.dx = cb3_element->IntAttribute("dx"); //delta to x
		CB3.dy = cb3_element->IntAttribute("dy"); //delta to y
	}
	else
		CB3.width = 0;
}

bool GameObject_Laser::turn(int time_elapsed)
{
	turn_time = time_elapsed;

	update_STATE();

	if(STATE == waiting ||
	   STATE == inactive)
		return false;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	if(STATE == splash && anim_pos >= spr_object[STATE]->get_frame_count())
	{
		if(!switch_state)
			laser_state = false; //if it was off then turn it off again
		set_STATE(waiting);
	}

	return move(time_elapsed);
}

void GameObject_Laser::show(int view_x, int view_y)
{
	if(STATE == waiting ||
	   STATE == inactive )
		return;

	//only render if in window
	if(!in_window(STATE))
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	//std::cerr << "LASER: anim_pos =" << anim_pos << " turn= " <<move_time<<" speed="<< speed <<"\n";
	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
