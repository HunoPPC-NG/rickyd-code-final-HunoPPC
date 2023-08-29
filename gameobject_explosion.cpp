/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_explosion.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Explosion construction:

GameObject_Explosion::GameObject_Explosion(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_STATE(waiting1);
	set_use_elevator(false);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/explosion/active/anim/sprite", world->global_resources);

	sfx_blast = new RE_SoundBuffer("global/objects/explosion/sounds/blast/sample", world->global_resources);
	sfx_blast->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);

	def_wait_time = EXPLOSION_WAIT;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("wait_time", &def_wait_time);
	}

	update_collision_box();

	turn_time = 0;
	anim_time = 0;
	wait_time = def_wait_time;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Explosion::~GameObject_Explosion()
{
	delete(spr_object[active]);
	delete(TriggerBox);

	delete(sound);
	delete(sfx_blast);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Explosion attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Explosion operations:
void GameObject_Explosion::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Explosion::check_trigger()
{
	bool trigger_set = false;

	if(TriggerBox)
	{
		if( world->bomb->get_type() == kill_all &&
			TriggerBox->is_overlapped(world->bomb->get_collision_rect()) )
		{
			trigger_set = true;
		}
	}

	return trigger_set;
}


void GameObject_Explosion::update_STATE()
{
	if(STATE == inactive)
		return;

	if(STATE == waiting1) //waiting for trigger
	{
		if(check_trigger())
		{
			set_STATE(waiting2);
		}
	}

	if(STATE == waiting2) //in start or end position
	{
		if(wait_time > 0)
		{
			wait_time -= turn_time;
		}
		else
		{
			set_STATE(active);
			if(!sound->is_playing())
				sound->play(sfx_blast);
		}
	}

}

void GameObject_Explosion::set_STATE(ObjectState state, bool final)
{
	if(state == active)
		set_type(kill_all);
	else
		set_type(kill_none);

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Explosion::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/explosion/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Explosion::turn(int time_elapsed)
{
	turn_time = time_elapsed;

	update_STATE();

	if(STATE != active)
		return false;

	if(anim_time < speed)
		anim_time += time_elapsed;
	else
		anim_time = 0;

	if(anim_time == 0) //rotate on turn start
		anim_pos++;

	int anim_length = spr_object[active]->get_frame_count();
	if (anim_pos > anim_length-1)
	{
		anim_pos=0;
		set_STATE(inactive);
	}

	bool ret = move(time_elapsed);

	return ret;
}

void GameObject_Explosion::show(int view_x, int view_y)
{
	if(STATE != active)
		return;
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
		(current_position->y - spr_object[active]->get_height() + 1) - view_y);
}
