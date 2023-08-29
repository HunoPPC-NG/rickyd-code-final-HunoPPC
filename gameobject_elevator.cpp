/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_elevator.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Elevator construction:

GameObject_Elevator::GameObject_Elevator(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(platform_elevator);
	set_use_elevator(false);
	set_STATE(waiting1);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/elevator/active/anim/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/elevator/waiting/anim/sprite"))
	{
		spr_object[waiting1] = new RE_Sprite(world->gc, "global/objects/elevator/waiting/anim/sprite", world->global_resources);
		spr_object[waiting2] = new RE_Sprite(world->gc, "global/objects/elevator/waiting/anim/sprite", world->global_resources);
	}
	else //use the active sprite
	{
		spr_object[waiting1] = new RE_Sprite(world->gc, "global/objects/elevator/active/anim/sprite", world->global_resources);
		spr_object[waiting2] = new RE_Sprite(world->gc, "global/objects/elevator/active/anim/sprite", world->global_resources);
	}
	sfx_moving = new RE_SoundBuffer("global/objects/elevator/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	initial_direction = object_element->FirstChildElement("position")->IntAttribute("state");
	direction = initial_direction;

	start.x = current_position->x;
	start.y = current_position->y;
	end.x = object_element->FirstChildElement("end_position")->IntAttribute("x");
	end.y = object_element->FirstChildElement("end_position")->IntAttribute("y");

	TriggerBox = NULL;
	SwitchBox = NULL;

	//check for switch_box
	tinyxml2::XMLElement* switch_element = NULL;
	switch_element = object_element->FirstChildElement("switch_box");
	if(switch_element)
	{
		int sb_x = switch_element->IntAttribute("x");
		int sb_y = switch_element->IntAttribute("y");
		int sb_width = switch_element->IntAttribute("width");
		int sb_height = switch_element->IntAttribute("height");

		set_switch_box(sb_x, sb_y, sb_width, sb_height);
	}

	//check for trigger_box
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

	update_collision_box();

	if(direction < 2)
		elevator_velocity = ELEVATOR_H_VELOCITY;
	else
		elevator_velocity = ELEVATOR_V_VELOCITY;

	elevator_return_velocity = elevator_velocity;
	def_wait = ELEVATOR_WAIT;
	def_cycles = ELEVATOR_CYCLES;
	def_trigger_delay = 0;

	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("velocity", &elevator_velocity);

		elevator_return_velocity = elevator_velocity;
		param_element->QueryIntAttribute("return_velocity", &elevator_return_velocity);

		param_element->QueryIntAttribute("wait_time", &def_wait);
		param_element->QueryIntAttribute("cycles", &def_cycles);
		param_element->QueryIntAttribute("trigger_delay", &def_trigger_delay);
	}

	returning = 0;
	turn_time = 0;
	move_time = 0;
	wait_time1 = def_wait;
	cycles = def_cycles;
	trigger_delay = def_trigger_delay;
	first_trigger = false;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Elevator::~GameObject_Elevator()
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
// GameObject_Elevator attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Elevator operations:
void GameObject_Elevator::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Elevator::check_trigger()
{
	if(TriggerBox)
	{
		if(world->player->get_type() != kill_none)
			return(TriggerBox->is_overlapped(world->player->get_collision_rect()));
	}
	else if (SwitchBox)
		return false; //if no trigger then elevator needs to be switched

	return true; //if no trigger and no switch elevator is active
}

void GameObject_Elevator::set_switch_box(int sb_x, int sb_y, int sb_width, int sb_height)
{
	SwitchBox = new RE_Rect(sb_x, sb_y - sb_height, sb_x + sb_width, sb_y);
}

bool GameObject_Elevator::check_switch()
{
	if(SwitchBox)
		return world->player->is_switch_trigger(SwitchBox);

	return false; //if no switch then elevator needs to be triggered
}

bool GameObject_Elevator::in_window(ObjectState state)
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

void GameObject_Elevator::update_STATE()
{
	if(STATE == waiting1) //waiting for trigger or switch
	{
		y_velocity= 0;
		if(check_switch() || check_trigger())
		{
			if(!first_trigger && trigger_delay > 0)
				trigger_delay -= turn_time;
			else
			{
				first_trigger = true;
				set_STATE(active);
			}
		}
	}

	if(STATE == waiting2) //in start or end position
	{
		if(wait_time1 > 0)
		{
			wait_time1 -= turn_time;
		}
		else
		{
			cycles--;
			if(cycles > 0)
				set_STATE(active);
			else
			{
				set_STATE(waiting1);
				cycles = def_cycles;
			}
		}
	}

	if(STATE == active)
	{
		if(!returning)
		{
			int curr_velocity = elevator_velocity;

			switch(direction)
			{
				case 0: // go left
				{
					y_velocity = 0;
					x_velocity = -curr_velocity;
					int left_limit = start.x;
					if(start.x > end.x)
						left_limit = end.x;
					if(current_position->x <= left_limit)
					{
						if(initial_direction == direction)
						{
							//check vertical movement
							if(start.y > end.y)
								direction = 2; //up
							else if(start.y < end.y)
								direction = 3; //down
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);
					}
				}
				break;
				case 1: //go right
				{
					y_velocity = 0;
					x_velocity = curr_velocity;
					int right_limit = end.x;
					if(start.x > end.x)
						right_limit = start.x;
					if(current_position->x >= right_limit)
					{
						if(initial_direction == direction)
						{
							//check vertical movement
							if(start.y > end.y)
								direction = 2; //up
							else if (start.y < end.y)
								direction = 3; //down
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);
					}
				}
				break;
				case 2: //go up
				{
					x_velocity = 0;
					y_velocity = -curr_velocity;
					int top_limit = end.y;
					if(start.y < end.y)
						top_limit = start.y;
					if(current_position->y <= top_limit)
					{
						if(initial_direction == direction)
						{
							//check horizontal movement
							if(start.x > end.x)
								direction = 0; //left
							else if(start.x < end.x)
								direction = 1; //right
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);
					}
				}
				break;
				default: //go down
				{
					x_velocity = 0;
					y_velocity = curr_velocity;
					int bottom_limit = start.y;
					if(start.y < end.y)
						bottom_limit = end.y;
					if(current_position->y >= bottom_limit)
					{
						if(initial_direction == direction)
						{
							//check horizontal movement
							if(start.x > end.x)
								direction = 0; //left
							else if(start.x < end.x)
								direction = 1; //right
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);

					}
				}
				break;
			}
		}
		else
		{
			int curr_velocity = elevator_return_velocity;

			switch(direction)
			{
				case 0: // go left
				{
					y_velocity = 0;
					x_velocity = -curr_velocity;
					int left_limit = start.x;
					if(start.x > end.x)
						left_limit = end.x;
					if(current_position->x <= left_limit)
					{
						if(initial_direction > 1) //up or down
						{
							//check vertical movement
							if(start.y > end.y)
								direction = 3; //down
							else if(start.y < end.y)
								direction = 2; //up
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);
					}
				}
				break;
				case 1: //go right
				{
					y_velocity = 0;
					x_velocity = curr_velocity;
					int right_limit = end.x;
					if(start.x > end.x)
						right_limit = start.x;
					if(current_position->x >= right_limit)
					{
						if(initial_direction > 1) //up or down
						{
							//check vertical movement
							if(start.y > end.y)
								direction = 3; //down
							else if(start.y < end.y)
								direction = 2; //up
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);
					}
				}
				break;
				case 2: //go up
				{
					x_velocity = 0;
					y_velocity = -curr_velocity;
					int top_limit = end.y;
					if(start.y < end.y)
						top_limit = start.y;
					if(current_position->y <= top_limit)
					{
						if(initial_direction < 2) //left or right
						{
							//check horizontal movement
							if(start.x > end.x)
								direction = 1; //right
							else if(start.x < end.x)
								direction = 0; //left
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);
					}
				}
				break;
				default: //go down
				{
					x_velocity = 0;
					y_velocity = curr_velocity;
					int bottom_limit = start.y;
					if(start.y < end.y)
						bottom_limit = end.y;
					if(current_position->y >= bottom_limit)
					{
						if(initial_direction < 2) //left or right
						{
							//check horizontal movement
							if(start.x > end.x)
								direction = 1; //right
							else if(start.x < end.x)
								direction = 0; //left
							else
								set_STATE(waiting2);
						}
						else
							set_STATE(waiting2);
					}
				}
				break;
			}
		}
	}
}

void GameObject_Elevator::set_STATE(ObjectState state, bool final)
{
	if(state == active)
	{
		wait_time1 = def_wait;
	}
	else if(state == waiting2) //invert direction
	{
		x_velocity = 0;
		y_velocity = 0;
		returning = !returning;
		switch(direction)
		{
			case 0:
				direction = 1; //go right
				break;
			case 1:
				direction = 0; //go left
				break;
			case 2:
				direction = 3; //go down
				break;
			default:
				direction = 2; //go up
				break;
		}
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Elevator::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/elevator/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Elevator::turn(int time_elapsed)
{
	update_STATE();

	turn_time = time_elapsed;

	bool ret = move(time_elapsed, false);

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	return ret;
}

void GameObject_Elevator::show(int view_x, int view_y)
{
	//only render if in window
	if(!in_window(STATE))
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		 current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() + 1) - view_y);
}
