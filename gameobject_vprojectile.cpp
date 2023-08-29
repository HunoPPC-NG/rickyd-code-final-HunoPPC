/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "gameobject_vprojectile.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_Vprojectile construction:

GameObject_Vprojectile::GameObject_Vprojectile(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	obj_name = object_name;

	set_type(kill_none);
	set_use_elevator(false);

	spr_object[moving_up] = NULL;
	spr_object[moving_down] = NULL;
	spr_object[splash_up] = NULL;
	spr_object[splash_down] = NULL;
	spr_object[waiting] = NULL;

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/moving/up/sprite"))
		spr_object[moving_up] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/moving/up/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/moving/down/sprite"))
		spr_object[moving_down] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/moving/down/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/splash/up/sprite"))
		spr_object[splash_up] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/splash/up/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/splash/down/sprite"))
		spr_object[splash_down] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/splash/down/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/waiting/sprite"))
		spr_object[waiting] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/waiting/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/moving/sample", world->global_resources);

	sfx_splash = NULL;

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/sounds/splash/sample"))
		sfx_splash = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/splash/sample", world->global_resources);

	sfx_moving->set_volume(1.0f);
	if(sfx_splash)
		sfx_splash->set_volume(1.0f);

	sound = new RE_SoundSession();

	proj_dir = -1; //move down multiple
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &proj_dir);

	if(proj_dir > 0 && !spr_object[moving_up])
	{
		std::cerr<< "GameObject_Vprojectile: (FATAL) direction is set to up but no matching sprite found\n";
		exit(-1);
	}

	if(proj_dir < 0 && !spr_object[moving_down])
	{
		std::cerr<< "GameObject_Vprojectile: (FATAL) direction is set to down but no matching sprite found\n";
		exit(-1);
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

	def_velocity = VPROJ_VELOCITY;
	def_trigger_time = VPROJ_TRIGGER_TIME;
	def_trigger_delay = 0;
	switch_state = 1; //active
	trigger_state = 0; //only player can trigger
	wall_collision = true;
	deadly_splash = false;

	int anim_velocity = 8;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("switch", &switch_state);
		param_element->QueryIntAttribute("trigger", &trigger_state);
		param_element->QueryIntAttribute("velocity", &def_velocity);
		param_element->QueryIntAttribute("anim_velocity", &anim_velocity);
		param_element->QueryIntAttribute("trigger_time", &def_trigger_time);
		param_element->QueryIntAttribute("trigger_delay", &def_trigger_delay);
		param_element->QueryIntAttribute("wall_collision", &wall_collision);
		param_element->QueryIntAttribute("deadly_splash", &deadly_splash);
	}

	if(switch_state > 0)
		is_on = true;
	else
		is_on = false;

	set_STATE(waiting);
	update_collision_box();

	anim_pos  = 0;
	turn_time = 0;
	move_time = 0;
	trigger_time = 0;
	trigger_delay = def_trigger_delay;
	triggered = false;
	set_speed(anim_velocity); //animation speed in frames per sec
}

GameObject_Vprojectile::~GameObject_Vprojectile()
{
	if(spr_object[moving_up])
		delete (spr_object[moving_up]);
	if(spr_object[moving_down])
		delete (spr_object[moving_down]);
	if(spr_object[splash_up])
		delete (spr_object[splash_up]);
	if(spr_object[splash_down])
		delete (spr_object[splash_down]);
	if(spr_object[waiting])
		delete (spr_object[waiting]);

	delete(sound);
	delete(sfx_moving);
	if(sfx_splash)
		delete(sfx_splash);

	if(TriggerBox)
		delete(TriggerBox);
	if(SwitchBox)
		delete(SwitchBox);

}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Vprojectile attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Vprojectile operations:

void GameObject_Vprojectile::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Vprojectile::check_trigger()
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

void GameObject_Vprojectile::set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	SwitchBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Vprojectile::check_switch()
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

bool GameObject_Vprojectile::in_window(ObjectState state)
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

void GameObject_Vprojectile::update_STATE()
{
	if( switch_state > 0 && check_switch()) //turn laser off
		is_on = false;
	else if(switch_state == 0 && check_switch()) //turn laser on
		is_on = true;

	if(trigger_time > 0)
	{
		trigger_time -= turn_time;
	}

	if(STATE == waiting && trigger_time <= 0)
	{
		if(is_on && (check_trigger() || triggered))
		{
			trigger_delay -=turn_time;

			if(trigger_delay <=0)
				triggered =  true;

			if(triggered)
			{
				if(proj_dir > 0)
					set_STATE(moving_up);
				else
					set_STATE(moving_down);
			}
		}

	}

	if(STATE == waiting ||
	   STATE == inactive)
		return;

	//check for wall hit
	if(STATE == moving_up)
	{
		if( collision_top ||
			(current_position->y < world->scroll_y + get_height()))
		{
			if(!spr_object[splash_up])
			{
				if(proj_dir > 1)
					set_STATE(inactive);
				else
					set_STATE(waiting); //repeat
			}
			else
				set_STATE(splash_up);
		}
	}
	else if (STATE == moving_down)
	{
		if( collision_bottom ||
			(current_position->y > world->scroll_y + world->gc->get_height()))
		{
			if(!spr_object[splash_down])
			{
				if(proj_dir < -1 )
					set_STATE(inactive);
				else
					set_STATE(waiting); //repeat
			}
			else
				set_STATE(splash_down);
		}
	}

	switch(STATE)
	{
		case moving_down:
			x_velocity = 0;
			y_velocity = def_velocity;
			break;

		case moving_up:
			x_velocity = 0;
			y_velocity = -def_velocity;
			break;

		case splash_up:
		case splash_down:
			x_velocity = 0;
			y_velocity = 0;
			break;
	}
}

void GameObject_Vprojectile::set_STATE(ObjectState state)
{
	switch(state)
	{
		case splash_up:
		case splash_down:
			anim_pos = 0;
			move_time = 0;
			if(deadly_splash > 0)
				set_type(kill_all);
			else
				set_type(kill_none);
			if(in_window(state))
			{
				sound->stop();
				if(sfx_splash)
					sound->play(sfx_splash);
			}
			//center splash
			{
			int m_width = 0;
			if(state == splash_up)
				m_width = spr_object[moving_up]->get_width();
			else
				m_width = spr_object[moving_down]->get_width();

			current_position->x -= (spr_object[state]->get_width() - m_width)/2;
			}
			collision_bottom = false;
			collision_top = false;
			break;

		case inactive:
		case waiting:
			set_type(kill_none);
			sound->stop();
			x_velocity = 0;
			y_velocity = 0;
			current_position->x = initial_x;
			current_position->y = initial_y;
			anim_pos = 0;
			collision_bottom = false;
			collision_top = false;
			break;

		default:
			set_type(kill_all);
			anim_pos = 0;
			trigger_time = def_trigger_time;
			if(!sound->is_playing()	&& in_window(state) )
				sound->play(sfx_moving);
			break;
	}

	previous_STATE = STATE;
	STATE = state;

	update_collision_box();
}


void GameObject_Vprojectile::update_collision_box()
{
	std::string action = "";
	switch(STATE)
	{
		case splash_up:
		case splash_down:
			action = "splash";
			break;

		case moving_up:
		case moving_down:
		default:
			action = "moving";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");

}

bool GameObject_Vprojectile::turn(int time_elapsed)
{
	turn_time = time_elapsed;

	update_STATE();

	if(STATE == waiting || STATE == inactive)
		return false;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	//std::cout<< "STATE:"<< STATE<< " anim_pos:"<< anim_pos<<"\n";
	if((STATE == splash_up || STATE == splash_down) &&
		anim_pos >= spr_object[STATE]->get_frame_count())
	{
		if(proj_dir < -1 || proj_dir > 1)
			set_STATE(inactive);
		else
			set_STATE(waiting); //repeat

		if(!switch_state)
			is_on = false; //if it was off then turn it off again
	}

	return move(time_elapsed, wall_collision);
}

void GameObject_Vprojectile::show(int view_x, int view_y)
{
	if( STATE == inactive )
		return;

	//only render if in window (checks for STATE sprite)
	if(!in_window(STATE))
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
