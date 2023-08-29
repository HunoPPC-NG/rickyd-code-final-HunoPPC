/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "gameobject_projectile.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_Projectile construction:

GameObject_Projectile::GameObject_Projectile(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	obj_name = object_name;
	set_use_elevator(false);

	set_type(kill_none);

	spr_object[moving_left] = NULL;
	spr_object[moving_right] = NULL;
	spr_object[splash_left] = NULL;
	spr_object[splash_right] = NULL;
	spr_object[waiting] = NULL;
	spr_object[delayed] = NULL;

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/moving/left/sprite"))
		spr_object[moving_left] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/moving/left/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/moving/right/sprite"))
		spr_object[moving_right] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/moving/right/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/splash/left/sprite"))
		spr_object[splash_left] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/splash/left/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/splash/right/sprite"))
		spr_object[splash_right] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/splash/right/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/waiting/sprite"))
		spr_object[waiting] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/waiting/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/delayed/sprite"))
		spr_object[delayed] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/delayed/sprite", world->global_resources);

	sfx_splash = NULL;

	sfx_proj = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/moving/sample", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/sounds/splash/sample"))
		sfx_splash = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/splash/sample", world->global_resources);

	sfx_proj->set_volume(1.0f);
	if(sfx_splash)
		sfx_splash->set_volume(1.0f);

	sound = new RE_SoundSession();

	proj_dir = -1; //move left multiple
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &proj_dir);

	if(proj_dir > 0 && !spr_object[moving_right])
	{
		std::cerr<< "GameObject_Projectile: (FATAL) direction is set to right but no matching sprite found\n";
		exit(-1);
	}

	if(proj_dir < 0 && !spr_object[moving_left])
	{
		std::cerr<< "GameObject_Projectile: (FATAL) direction is set to left but no matching sprite found\n";
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
	def_velocity = PROJ_VELOCITY;
	def_trigger_time = PROJ_TRIGGER_TIME;
	def_trigger_delay1 = 0;
	def_trigger_delay2 = 0;
	switch_state = 1;
	wall_collision = true;
	deadly_splash = false;

	int anim_velocity = 8;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("switch", &switch_state);
		param_element->QueryIntAttribute("velocity", &def_velocity);
		param_element->QueryIntAttribute("anim_velocity", &anim_velocity);
		param_element->QueryIntAttribute("trigger_time", &def_trigger_time);
		param_element->QueryIntAttribute("trigger_delay", &def_trigger_delay1);
		param_element->QueryIntAttribute("trigger_delay2", &def_trigger_delay2);
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
	trigger_delay = 0;
	delay_ind = 0;
	set_speed(anim_velocity); //animation speed in frames per sec
}

GameObject_Projectile::~GameObject_Projectile()
{
	if(spr_object[moving_left])
		delete (spr_object[moving_left]);
	if(spr_object[moving_right])
		delete (spr_object[moving_right]);
	if(spr_object[splash_left])
		delete (spr_object[splash_left]);
	if(spr_object[splash_right])
		delete (spr_object[splash_right]);
	if(spr_object[waiting])
		delete (spr_object[waiting]);

	delete(sound);
	delete(sfx_proj);
	if(sfx_splash)
		delete(sfx_splash);

	if(TriggerBox)
		delete(TriggerBox);
	if(SwitchBox)
		delete(SwitchBox);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Projectile attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Projectile operations:

void GameObject_Projectile::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Projectile::check_trigger()
{
	if( TriggerBox != NULL &&
		world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));
	else if (!TriggerBox)
		return true;

	return false;
}

void GameObject_Projectile::set_switch_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	SwitchBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Projectile::check_switch()
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

bool GameObject_Projectile::in_window(ObjectState state)
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

void GameObject_Projectile::update_STATE()
{
	if( switch_state > 0 && check_switch()) //turn projectile off
		is_on = false;
	else if(switch_state == 0 && check_switch()) //turn projectile on
		is_on = true;

	if(STATE == waiting && trigger_time <= 0)
	{
		if(is_on && check_trigger())
		{
			if(delay_ind > 0)
				trigger_delay = def_trigger_delay2;
			else if(delay_ind == 0)
				trigger_delay = def_trigger_delay1;
			else
				trigger_delay = 0; //no more delay

			trigger_time = def_trigger_time;

			set_STATE(delayed);
		}
		else
		{
			delay_ind = 0; //not triggered restart delay
		}
	}

	if(STATE == delayed && trigger_delay <= 0)
	{
		if(delay_ind > 0)
			delay_ind = 0;
		else
		{
			if(def_trigger_delay2 > 0)
				delay_ind = 1;
			else if(def_trigger_delay2 == 0)
				delay_ind = 0;
			else
				delay_ind = -1;//no more delay
		}

		if(proj_dir > 0)
			set_STATE(moving_right);
		else
			set_STATE(moving_left);
	}

	if(trigger_delay > 0)
	{
		trigger_delay -= turn_time;
	}

	if(trigger_time > 0)
	{
		trigger_time -= turn_time;
	}

	if(STATE == inactive ||
	   STATE == waiting ||
	   STATE == delayed)
		return;

	//check for wall hit
	if(STATE == moving_left)
	{
		if( collision_left ||
			(current_position->x < world->scroll_x))
		{
			if(!spr_object[splash_left])
			{
				if(proj_dir < -1)
					set_STATE(inactive);
				else
					set_STATE(waiting); //repeat
			}
			else
				set_STATE(splash_left);
		}
	}
	else if(STATE == moving_right)
	{
		if( collision_right ||
			(current_position->x > world->scroll_x + world->gc->get_width() - get_width()))
		{
			if(!spr_object[splash_right])
			{
				if(proj_dir > 1)
					set_STATE(inactive);
				else
					set_STATE(waiting); //repeat
			}
			else
				set_STATE(splash_right);
		}
	}

	switch(STATE)
	{
		case moving_left:
			x_velocity = -def_velocity;
			y_velocity = 0;
			break;

		case moving_right:
			x_velocity = def_velocity;
			y_velocity = 0;
			break;

		case splash_left:
		case splash_right:
			x_velocity = 0;
			y_velocity = 0;
			break;
	}
}


void GameObject_Projectile::set_STATE(ObjectState state)
{
	//std::cout<< "STATE:"<< STATE<< " state:"<< state<<"\n";
	switch(state)
	{
		case splash_left:
		case splash_right:
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
			int m_height = 0;
			if(state == splash_left)
				m_height = spr_object[moving_left]->get_height();
			else
				m_height = spr_object[moving_right]->get_height();

			current_position->y += (spr_object[state]->get_height() - m_height)/2;
			}
			collision_left = false;
			collision_right = false;
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
			collision_left = false;
			collision_right = false;
			break;

		default:
			set_type(kill_all);
			anim_pos = 0;
			trigger_time = def_trigger_time;
			if(!sound->is_playing()	&& in_window(state) )
				sound->play(sfx_proj);
			break;
	}

	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Projectile::update_collision_box()
{
	std::string action = "";
	switch(STATE)
	{
		case splash_left:
		case splash_right:
			action = "splash";
			break;

		case moving_left:
		case moving_right:
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

bool GameObject_Projectile::turn(int time_elapsed)
{
	turn_time = time_elapsed;

	update_STATE();

	y_velocity = 0;

	if(STATE == waiting || STATE == inactive)
		return false;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	//std::cout<< "STATE:"<< STATE<< " anim_pos:"<< anim_pos<<"\n";
	if((STATE == splash_left || STATE == splash_right) &&
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

void GameObject_Projectile::show(int view_x, int view_y)
{
	if( STATE == inactive )
		return;

	//only render if in window
	if(!in_window(STATE))
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
