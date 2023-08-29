/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_flame.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Flame construction:

GameObject_Flame::GameObject_Flame(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(kill_none);
	set_use_elevator(false);

	spr_object[big_right] = new RE_Sprite(world->gc, "global/objects/flame/big/right/sprite", world->global_resources);
	spr_object[big_left] = new RE_Sprite(world->gc, "global/objects/flame/big/left/sprite", world->global_resources);
	spr_object[small_right] = new RE_Sprite(world->gc, "global/objects/flame/small/right/sprite", world->global_resources);
	spr_object[small_left] = new RE_Sprite(world->gc, "global/objects/flame/small/left/sprite", world->global_resources);

	sfx_flame = new RE_SoundBuffer("global/objects/flame/sounds/shoot/sample", world->global_resources);
	sfx_flame->set_volume(1.0f);

	sound = new RE_SoundSession();

	pos_x = object_element->FirstChildElement("position")->IntAttribute("x");
	pos_y = object_element->FirstChildElement("position")->IntAttribute("y");
	current_position->x = pos_x;
	current_position->y = pos_y;
	direction =  object_element->FirstChildElement("position")->IntAttribute("state");

	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);

	set_STATE(waiting);
	update_collision_box();

	x_velocity = 0;
	y_velocity = 0;

	anim_pos = 0;
	turn_time = 0;
	set_speed(5); //animation speed in frames per sec

}

GameObject_Flame::~GameObject_Flame()
{
	delete(spr_object[big_right]);
	delete(spr_object[big_left]);
	delete(spr_object[small_right]);
	delete(spr_object[small_left]);
	delete(sfx_flame);
	delete(sound);
	delete(TriggerBox);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Flame attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Flame operations:

void GameObject_Flame::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Flame::check_trigger()
{
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}

void GameObject_Flame::update_STATE()
{
	if(STATE == inactive)
		return;

	if(STATE == waiting)
	{
		if(check_trigger())
		{
			switch(direction)
			{
				case 0:
					set_STATE(big_right);
					break;
				default:
					set_STATE(big_left);
					break;
			}

			sound->play(sfx_flame);
			anim_pos = 0;
			turn_time = 0;
		}
	}

	if( STATE != waiting && anim_pos > spr_object[STATE]->get_frame_count()-1)
	{
		switch(STATE)
		{
			case big_right:
				set_STATE(small_right);
				break;
			case big_left:
				set_STATE(small_left);
				break;
			case small_right:
				set_STATE(big_right);
				break;
			case small_left:
				set_STATE(big_left);
				break;
		}

		anim_pos = 0;
		turn_time = 0;
	}

}


void GameObject_Flame::set_STATE(ObjectState state)
{
	if(state != waiting)
	{
		set_type(kill_all);
	}

	switch(state)
	{
		case big_left:
			current_position->x = pos_x - spr_object[big_left]->get_width();
			current_position->y = pos_y + spr_object[big_left]->get_height()/2;
			break;
		case small_left:
			current_position->x = pos_x - spr_object[small_left]->get_width();
			current_position->y = pos_y + spr_object[big_right]->get_height()/4;
			break;
		case big_right:
			current_position->y = pos_y + spr_object[big_right]->get_height()/2;
			current_position->y = pos_y + spr_object[big_right]->get_height()/2;
			break;
		case small_right:
			current_position->y = pos_y + spr_object[big_right]->get_height()/4;
			break;
	}

	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Flame::update_collision_box()
{
	std::string action;

	switch(STATE)
	{
		case big_left:
		case big_right:
			action = "big";
			break;
		default:
			action = "small";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/flame/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Flame::turn(int time_elapsed)
{
	if(STATE == inactive)
		return false;

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	update_STATE();
	update_collision_box();

	return move(time_elapsed);
}

void GameObject_Flame::show(int view_x, int view_y)
{
	if(STATE == inactive || STATE == waiting)
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

