/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_bat.h"
#include "gameobject_player.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Bat construction:

GameObject_Bat::GameObject_Bat(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(kill_all);

	spr_object[resting] = new RE_Sprite(world->gc, "global/objects/bat/resting/anim/sprite", world->global_resources);
	spr_object[moving]  = new RE_Sprite(world->gc, "global/objects/bat/moving/anim/sprite", world->global_resources);

	anim_pos = 0;
	wait_time = WAIT_TIME;

	i_am_dead = false;
	//got_powerup = false;

	set_speed(4); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	start_x = current_position->x;
	start_y = current_position->y;
	end_x = object_element->FirstChildElement("end_position")->IntAttribute("x");
	end_y = object_element->FirstChildElement("end_position")->IntAttribute("y");

	yvel = end_y - start_y;
	xvel = end_x - start_x;

	if(fabs(xvel) > fabs(yvel))
	{
		xvel /= fabs(yvel);
		yvel /= fabs(yvel);
	}
	else
	{
		yvel /= fabs(xvel);
		xvel /= fabs(xvel);
	}
	xvel *= BAT_VELOCITY;
	yvel *= BAT_VELOCITY;
	xvel = ceil(xvel);
	yvel = ceil(yvel);

	//std::cout << "Bat: xvel:" << xvel <<" yvel:"<<yvel<<"\n";
	set_STATE(resting);
	update_collision_box();

	turn_time = 0;
	move_time = 0;
}

GameObject_Bat::~GameObject_Bat()
{
	delete(spr_object[resting]);
	delete(spr_object[moving]);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Bat attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Bat operations:

void GameObject_Bat::update_STATE()
{
	//check for stick collision
	GameObject_Player *player = (GameObject_Player *) world->player;
	if((STATE == resting) &&
	  ((world->check_collision(kill_all, this) ||
	    player->is_stick(get_collision_rect()))))
	{
		set_STATE(moving);
	}


	if(STATE == moving)
	{
		if(wait_time > 0)
		{
			if(xvel > 0)
			{
				if(current_position->x < end_x)
					x_velocity = xvel;
				else
					x_velocity = 0;
			}
			else
			{
				if(current_position->x > end_x)
					x_velocity = xvel;
				else
					x_velocity = 0;
			}

			if(yvel > 0)
			{
				if(current_position->y < end_y)
					y_velocity = yvel;
				else
					y_velocity = 0;
			}
			else
			{
				if(current_position->y > end_y)
					y_velocity = yvel;
				else
					y_velocity = 0;
			}

			if( !(y_velocity || x_velocity))
				wait_time -= turn_time;

		}
		else //revert to original positiion
		{
			if(xvel > 0)
			{
				if(current_position->x > start_x)
					x_velocity = -xvel;
				else
					x_velocity = 0;
			}
			else
			{
				if(current_position->x < start_x)
					x_velocity = -xvel;
				else
					x_velocity = 0;
			}

			if(yvel > 0)
			{
				if(current_position->y > start_y)
					y_velocity = -yvel;
				else
					y_velocity = 0;
			}
			else
			{
				if(current_position->y < start_y)
					y_velocity = -yvel;
				else
					y_velocity = 0;
			}

			if( !(y_velocity || x_velocity))
				set_STATE(resting);
		}
	}
}

void GameObject_Bat::deploy(int start_x, int start_y, int state)
{
	if(state >= 0)
	{
		current_position->x = start_x;
		current_position->y = start_y;
	}
	else
	{
		current_position->x += start_x;
		current_position->y += start_y;
	}
}

void GameObject_Bat::set_STATE(ObjectState state, bool final)
{
	if(state == resting)
	{
		wait_time = WAIT_TIME;
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Bat::update_collision_box()
{
	std::string action;

	switch(STATE)
	{
		case resting:
			action = "resting";
			break;
		case moving:
			action = "moving";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/bat/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Bat::turn(int time_elapsed)
{
	turn_time = time_elapsed;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	bool ret = move(time_elapsed, false);

	update_STATE();

	//update_collision_box();

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	return ret;
}

void GameObject_Bat::show(int view_x, int view_y)
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
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
