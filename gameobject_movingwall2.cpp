/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_movingwall2.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Caveman1 construction:

GameObject_Movingwall2::GameObject_Movingwall2(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(wall);
	set_use_elevator(false);
	set_STATE(active);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/movingwall2/active/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/movingwall2/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	check_wall = false; //don't check wall collision on fist move

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	start_x = current_position->x;
	start_y = current_position->y;
	end_x = object_element->FirstChildElement("end_position")->IntAttribute("x");
	end_y = object_element->FirstChildElement("end_position")->IntAttribute("y");

	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);

	update_collision_box();

	turn_time = 0;
	trigger_time = MOVINGBLOCK2_TRIGGER_TIME;
	wait_time1 = MOVINGBLOCK2_WAIT1;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Movingwall2::~GameObject_Movingwall2()
{
	delete(spr_object[active]);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall2 attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall2 operations:
void GameObject_Movingwall2::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Movingwall2::check_trigger()
{
	//check gameobject kill_player type collision
	if(world->check_box_collision(kill_player, TriggerBox))
		return true;
	//check player collision
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}


void GameObject_Movingwall2::update_STATE()
{
	if(get_type() == wall)
	{
		if(trigger_time >= MOVINGBLOCK2_TRIGGER_TIME)
		{
			if(check_trigger())
				trigger_time -= turn_time;
		}
		else if(trigger_time > 0)
		{
			trigger_time -= turn_time;
		}
		else if(wait_time1 >= MOVINGBLOCK2_WAIT1)
		{
			//move to end position
			set_type(kill_all);
		}
		else if(wait_time1 > 0)
		{
			wait_time1 -= turn_time;
		}
		else
		{
			//move to start position
			set_type(kill_all);
		}
	}

	if(get_type() == kill_all)
	{
		y_velocity = 0;
		x_velocity = 0;

		if(wait_time1 >= MOVINGBLOCK2_WAIT1)
		{
			//move to end position (x first)
			if(start_x < end_x && current_position->x < end_x)
				x_velocity = MOVINGBLOCK2_H_VELOCITY*2;
			else if(start_x > end_x && current_position->x > end_x)
				x_velocity = -MOVINGBLOCK2_H_VELOCITY*2;

			if(!x_velocity /*|| collision_right || collision_left*/)
			{
				//move to end position (y last)
				if(start_y < end_y && current_position->y < end_y)
					y_velocity += MOVINGBLOCK2_V_VELOCITY*2; // go increasingly faster here
				else if(start_y > end_y && current_position->y > end_y)
					y_velocity = -MOVINGBLOCK2_V_VELOCITY*2;

				if(!y_velocity)
				{
					//stop moving
					wait_time1 -= turn_time;
					set_type(wall);
				}
			}
		} //start return movement
		else
		{
			//move to start position (x first)
			if(end_x < start_x && current_position->x < start_x)
				x_velocity = MOVINGBLOCK2_H_VELOCITY;
			else if(end_x > start_x && current_position->x > start_x)
				x_velocity = -MOVINGBLOCK2_H_VELOCITY;

			if(!x_velocity /*|| collision_right || collision_left*/)
			{
				//move to start position (y last)
				if(end_y < start_y && current_position->y < start_y)
					y_velocity = MOVINGBLOCK2_V_VELOCITY;
				else if(end_y > start_y && current_position->y > start_y)
					y_velocity = -MOVINGBLOCK2_H_VELOCITY;

				if(!y_velocity)
				{
					//stop moving
					wait_time1 = MOVINGBLOCK2_WAIT1;
					trigger_time = MOVINGBLOCK2_TRIGGER_TIME;
					set_type(wall);
					check_wall = false;
				}
			}
		}
	}
}

void GameObject_Movingwall2::set_STATE(ObjectState state, bool final)
{
	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Movingwall2::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/movingwall2/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Movingwall2::turn(int time_elapsed)
{
	update_STATE();

	turn_time = time_elapsed;

	bool ret = move(time_elapsed, check_wall);

	if(abs(y_velocity) > 1 || abs(x_velocity) > 1 )
		check_wall = true;

	//update_collision_box();

	anim_pos=0;

	return ret;
}

void GameObject_Movingwall2::show(int view_x, int view_y)
{
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
		(current_position->y - spr_object[STATE]->get_height() + 1) - view_y);
}
