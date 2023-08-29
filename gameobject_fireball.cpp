/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_fireball.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Caveman1 construction:

GameObject_Fireball::GameObject_Fireball(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(kill_none);
	set_use_elevator(false);
	set_STATE(waiting);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/fireball/active/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/fireball/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	center_x = object_element->FirstChildElement("position")->IntAttribute("x");
	center_y = object_element->FirstChildElement("position")->IntAttribute("y");
	radius = object_element->FirstChildElement("position")->IntAttribute("radius");
	direction = object_element->FirstChildElement("position")->IntAttribute("state");

	if(direction > 1)
		initial_angle = 3*PI/2;
	else
		initial_angle = PI/2;

	angle = initial_angle;
	current_position->x = center_x + radius * cos(angle);
	current_position->y = center_y + radius * sin(angle);


	int tb_x = object_element->FirstChildElement("trigger_box")->IntAttribute("x");
	int tb_y = object_element->FirstChildElement("trigger_box")->IntAttribute("y");
	int tb_width = object_element->FirstChildElement("trigger_box")->IntAttribute("width");
	int tb_height = object_element->FirstChildElement("trigger_box")->IntAttribute("height");

	set_trigger_box(tb_x, tb_y, tb_width, tb_height);

	update_collision_box();

	turn_time = 0;
	rotations = 0;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Fireball::~GameObject_Fireball()
{
	delete(spr_object[active]);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Fireball attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Fireball operations:
void GameObject_Fireball::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Fireball::check_trigger()
{
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}


void GameObject_Fireball::update_STATE()
{
	if(STATE == inactive)
		return;

	if(STATE == waiting)
	{
		if(check_trigger())
				set_STATE(active);
	}

	if(STATE == active)
	{
		if((current_position->x > world->gc->get_width() - (get_width() + floor(world->map->get_tile_width()/2))) ||
		   (current_position->x < floor(world->map->get_tile_width()/2)))
		{
			set_STATE(inactive);
		}
		else if(rotations < FIREBALL_ROTATIONS)
		{
			//rotate around center
			if(direction > 0)
			{
				angle -= FIREBALL_RAD_VELOCITY;
				if(angle <=  initial_angle - 2*PI)
				{
					rotations++;
					angle = initial_angle;
				}
			}
			else
			{
				angle += FIREBALL_RAD_VELOCITY;
				if(angle >= initial_angle + 2*PI)
				{
					rotations++;
					angle = initial_angle;
				}
			}
			current_position->x = (int) center_x + radius * cos(angle);
			current_position->y = (int) center_y + radius * sin(angle);
		}
		else
		{
			if(direction > 0)
			{
				x_velocity = -FIREBALL_H_VELOCITY;
			}
			else
			{
				x_velocity = FIREBALL_H_VELOCITY;
			}
		}

	}

}

void GameObject_Fireball::set_STATE(ObjectState state, bool final)
{
	if(state == active)
		set_type(kill_all);
	else
		set_type(kill_none);

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Fireball::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/fireball/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Fireball::turn(int time_elapsed)
{
	update_STATE();

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	bool ret = false;

	if(STATE == active)
		ret = move(time_elapsed, false);

	return ret;
}

void GameObject_Fireball::show(int view_x, int view_y)
{
	if(STATE != active)
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
		(current_position->y - spr_object[STATE]->get_height() + 1) - view_y);
}
