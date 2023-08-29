/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_movingwall4.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall4 construction:

GameObject_Movingwall4::GameObject_Movingwall4(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(wall_elevator);
	set_use_elevator(false);
	set_STATE(waiting1);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/movingwall4/active/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/movingwall4/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	start_x = current_position->x;
	start_y = current_position->y;
	mid_x = object_element->FirstChildElement("mid_position")->IntAttribute("x");
	mid_y = object_element->FirstChildElement("mid_position")->IntAttribute("y");
	end_x = object_element->FirstChildElement("end_position")->IntAttribute("x");
	end_y = object_element->FirstChildElement("end_position")->IntAttribute("y");

	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);

	update_collision_box();

	turn_time = 0;
	wait_time1 = MOVINGBLOCK4_WAIT1;
	wait_time2 = MOVINGBLOCK4_WAIT2;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Movingwall4::~GameObject_Movingwall4()
{
	delete(spr_object[active]);
	delete(TriggerBox);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall4 attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall4 operations:
void GameObject_Movingwall4::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Movingwall4::check_trigger()
{
	//check gameobject kill_all type collision (bullet, bomb, mummy)
	if(world->check_box_collision(kill_all, TriggerBox))
		return true;
	//check player collision
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}


void GameObject_Movingwall4::update_STATE()
{
	if(STATE == waiting1) //waiting for trigger
	{
		y_velocity= 0;
		if(check_trigger())
		{
			set_STATE(active);
		}
	}

	if(STATE == waiting2) //in mid position
	{
		if(wait_time1 > 0)
		{
			wait_time1 -= turn_time;
		}
		else
		{
			set_STATE(active);
		}
	}

	if(STATE == waiting3) //in end position
	{
		if(wait_time2 > 0)
		{
			wait_time2 -= turn_time;
		}
		else
		{
			set_STATE(active);
		}
	}

	if(STATE == active)
	{

		if(wait_time1 > 0)
		{
			//y first
			if((mid_y > start_y && current_position->y >= mid_y) ||
			   (mid_y <= start_y && current_position->y <= mid_y))
			{
				y_velocity = 0;
			}
			else
			{
				if(mid_y < start_y)
					y_velocity = -MOVINGBLOCK4_V_VELOCITY;
				else
					y_velocity = MOVINGBLOCK4_V_VELOCITY;
			}
			//x last
			if((mid_x > start_x && current_position->x >= mid_x) ||
			   (mid_x <= start_x && current_position->x <= mid_x))
			{
				x_velocity = 0;
			}
			else
			{
				if(mid_x < start_x)
					x_velocity = -MOVINGBLOCK4_V_VELOCITY;
				else
					x_velocity = MOVINGBLOCK4_V_VELOCITY;
			}

			if(!(x_velocity || y_velocity))
				set_STATE(waiting2);
		}
		else if(wait_time2 > 0)
		{
			//y first
			if((end_y > mid_y && current_position->y >= end_y) ||
			   (end_y <= mid_y && current_position->y <= end_y))
			{
				y_velocity = 0;
			}
			else
			{
				if(end_y < mid_y)
					y_velocity = -MOVINGBLOCK4_V_VELOCITY;
				else
					y_velocity = MOVINGBLOCK4_V_VELOCITY;
			}
			//x last
			if((end_x > mid_x && current_position->x >= end_x) ||
			   (end_x <= mid_x && current_position->x <= end_x))
			{
				x_velocity = 0;
			}
			else
			{
				if(end_x < mid_x)
					x_velocity = -MOVINGBLOCK4_V_VELOCITY;
				else
					x_velocity = MOVINGBLOCK4_V_VELOCITY;
			}

			if(!(x_velocity || y_velocity))
				set_STATE(waiting3);
		}
		else //go back at double speed
		{
			//y first
			if(start_y < end_y)
			{
				if(!collision_top && current_position->y > start_y)
					y_velocity = -2*MOVINGBLOCK4_V_VELOCITY;
				else
					y_velocity = 0;
			}
			else
			{
				if(!collision_bottom && current_position->y < start_y)
					y_velocity = 2*MOVINGBLOCK4_V_VELOCITY;
				else
					y_velocity = 0;
			}

			//x last
			if(start_x < end_x)
			{
				if(!collision_left && current_position->x > start_x)
					x_velocity = -2*MOVINGBLOCK4_V_VELOCITY;
				else
					x_velocity = 0;
			}
			else
			{
				if(!collision_right && current_position->x < start_x)
					x_velocity = 2*MOVINGBLOCK4_V_VELOCITY;
				else
					x_velocity = 0;
			}

			if(!(x_velocity || y_velocity)) //restart
			{
				set_STATE(waiting1);
				wait_time1 = MOVINGBLOCK4_WAIT1;
				wait_time2 = MOVINGBLOCK4_WAIT2;
			}
		}
	}
}

void GameObject_Movingwall4::set_STATE(ObjectState state, bool final)
{
	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Movingwall4::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/movingwall4/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Movingwall4::turn(int time_elapsed)
{
	update_STATE();

	turn_time = time_elapsed;

	bool ret = move(time_elapsed);

	anim_pos=0;

	return ret;
}

void GameObject_Movingwall4::show(int view_x, int view_y)
{
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
