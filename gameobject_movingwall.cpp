/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_movingwall.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Caveman1 construction:

GameObject_Movingwall::GameObject_Movingwall(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(wall);
	set_use_elevator(false);
	set_STATE(active);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/movingwall/active/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/movingwall/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	start_x = current_position->x;
	start_y = current_position->y;
	end_x = object_element->FirstChildElement("end_position")->IntAttribute("x");
	end_y = object_element->FirstChildElement("end_position")->IntAttribute("y");
	mid1_x = object_element->FirstChildElement("mid1_position")->IntAttribute("x");
	mid1_y = object_element->FirstChildElement("mid1_position")->IntAttribute("y");
	mid2_x = object_element->FirstChildElement("mid2_position")->IntAttribute("x");
	mid2_y = object_element->FirstChildElement("mid2_position")->IntAttribute("y");

	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);

	update_collision_box();

	turn_time = 0;
	wait_time1 = MOVINGBLOCK_WAIT1;
	wait_time2 = MOVINGBLOCK_WAIT2;
	wait_time3 = MOVINGBLOCK_WAIT3;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Movingwall::~GameObject_Movingwall()
{
	delete(spr_object[active]);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall operations:
void GameObject_Movingwall::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Movingwall::check_trigger()
{
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}


void GameObject_Movingwall::update_STATE()
{
	if(get_type() == wall)
	{
		if(wait_time1 >= MOVINGBLOCK_WAIT1)
		{
			if(check_trigger())
				set_type(kill_all);
		}
		else if(wait_time1 > 0)
		{
			wait_time1 -= turn_time;
		}
		else if(wait_time2 >= MOVINGBLOCK_WAIT2)
		{
			//move to middle position
			set_type(kill_all);
		}
		else if(wait_time2 > 0)
		{
			wait_time2 -= turn_time;
		}
		else if(wait_time3 >= MOVINGBLOCK_WAIT3)
		{
			//move to middle position
			set_type(kill_all);
		}
		else if(wait_time3 > 0)
		{
			wait_time3 -= turn_time;
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

		if(wait_time1 >= MOVINGBLOCK_WAIT1)
		{
			//move to end position (y first)
			if(start_y < end_y && current_position->y < end_y)
				y_velocity = MOVINGBLOCK_V_VELOCITY*4; // go faster here
			else if(start_y > end_y && current_position->y > end_y)
				y_velocity = -MOVINGBLOCK_V_VELOCITY*4;

			if(!y_velocity || collision_bottom || collision_top)
			{
				//move to end position (x last)
				if(start_x < end_x && current_position->x < end_x)
					x_velocity = MOVINGBLOCK_H_VELOCITY;
				else if(start_x > end_x && current_position->x > end_x)
					x_velocity = -MOVINGBLOCK_H_VELOCITY;

				if(!x_velocity)
				{
					//stop moving
					wait_time1 -= turn_time;
					set_type(wall);
				}
			}
		} //start return movement
		else if(wait_time2 >= MOVINGBLOCK_WAIT2)
		{
			//move to mid1 position (x first)
			if(end_x < mid1_x && current_position->x < mid1_x)
				x_velocity = MOVINGBLOCK_H_VELOCITY;
			else if(end_x > mid1_x && current_position->x > mid1_x)
				x_velocity = -MOVINGBLOCK_H_VELOCITY;

			if(!x_velocity || collision_left || collision_right)
			{
				//move to mid1 position (y last)
				if(end_y < mid1_y && current_position->y < mid1_y)
					y_velocity = MOVINGBLOCK_V_VELOCITY;
				else if(end_y > mid1_y && current_position->y > mid1_y)
					y_velocity = -MOVINGBLOCK_V_VELOCITY;

				if(!y_velocity)
				{
					//stop moving
					wait_time2 -= turn_time;
					set_type(wall);
				}
			}
		}
		else if(wait_time3 >= MOVINGBLOCK_WAIT3)
		{
			//move to mid2 position (x first)
			if(mid1_x < mid2_x && current_position->x < mid2_x)
				x_velocity = MOVINGBLOCK_H_VELOCITY;
			else if(mid1_x > mid2_x && current_position->x > mid2_x)
				x_velocity = -MOVINGBLOCK_H_VELOCITY;

			if(!x_velocity || collision_left || collision_right)
			{
				//move to mid2 position (y last)
				if(mid1_y < mid2_y && current_position->y < mid2_y)
					y_velocity = MOVINGBLOCK_V_VELOCITY;
				else if(mid1_y > mid2_y && current_position->y > mid2_y)
					y_velocity = -MOVINGBLOCK_V_VELOCITY;

				if(!y_velocity)
				{
					//stop moving
					wait_time3 -= turn_time;
					set_type(wall);
				}
			}
		}
		else
		{
			//move to start position (y first)
			if(mid2_y < start_y && current_position->y < start_y)
				y_velocity = MOVINGBLOCK_V_VELOCITY;
			else if(mid2_y > start_y && current_position->y > start_y)
				y_velocity = -MOVINGBLOCK_H_VELOCITY;

			if(!y_velocity || collision_top || collision_bottom)
			{
				std::cout << "x:"<< current_position->x << " y:"<< current_position->y <<"\n";
				//move to start position (x last)
				if(mid2_x < start_x && current_position->x < start_x)
					x_velocity = MOVINGBLOCK_H_VELOCITY;
				else if(mid2_x > start_x && current_position->x > start_x)
					x_velocity = -MOVINGBLOCK_H_VELOCITY;

				if(!x_velocity)
				{
					//stop moving
					wait_time1 = MOVINGBLOCK_WAIT1;
					wait_time2 = MOVINGBLOCK_WAIT2;
					wait_time3 = MOVINGBLOCK_WAIT3;
					set_type(wall);
				}
			}
		}
	}
}

void GameObject_Movingwall::set_STATE(ObjectState state, bool final)
{
	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Movingwall::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/movingwall/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Movingwall::turn(int time_elapsed)
{
	update_STATE();

	turn_time = time_elapsed;

	bool ret = move(time_elapsed);

	//update_collision_box();

	anim_pos=0;

	return ret;
}

void GameObject_Movingwall::show(int view_x, int view_y)
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
