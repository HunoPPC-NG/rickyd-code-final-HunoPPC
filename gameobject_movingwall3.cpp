/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_movingwall3.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Caveman1 construction:

GameObject_Movingwall3::GameObject_Movingwall3(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(wall_elevator);
	set_use_elevator(false);
	set_STATE(waiting1);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/movingwall3/active/anim/sprite", world->global_resources);

	sfx_moving = new RE_SoundBuffer("global/objects/movingwall3/sounds/moving/sample", world->global_resources);
	sfx_moving->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;

	i_am_dead = false;

	check_wall = false; //don't check wall collision on fist move

	set_speed(5); //animation speed in frames per sec

	def_trigger_time = MOVINGBLOCK3_TRIGGER_TIME;
	def_wait_time1 = MOVINGBLOCK3_WAIT1;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("delay_time", &def_trigger_time);
		param_element->QueryIntAttribute("return_time", &def_wait_time1);
	}

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
	trigger_time = def_trigger_time;
	wait_time1 = def_wait_time1;

	x_velocity = 0;
	y_velocity = 0;
}

GameObject_Movingwall3::~GameObject_Movingwall3()
{
	delete(spr_object[active]);

	delete(sound);
	delete(sfx_moving);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall3 attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Movingwall3 operations:
void GameObject_Movingwall3::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Movingwall3::check_trigger()
{
	//check gameobject kill_player type collision
	if(world->check_box_collision(kill_player, TriggerBox))
		return true;
	//check player collision
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}


void GameObject_Movingwall3::update_STATE()
{

	if(collision_bottom || current_position->y >= end_y)
		set_STATE(waiting3);

	if(STATE == waiting1)
	{
		y_velocity= 0;
		if(check_trigger())
			set_STATE(waiting2);
	}

	if(STATE == waiting2)
	{
		y_velocity = 0;
		if(trigger_time > 0)
			trigger_time -= turn_time;
		else if(trigger_time <= 0)
			set_STATE(active);
	}

	if(STATE == active)
	{
		y_velocity = MOVINGBLOCK3_V_VELOCITY*4;
		trigger_time = def_trigger_time;
		set_STATE(waiting2);
	}

	if(STATE == waiting3)
	{
		if(wait_time1 > 0)
			wait_time1 -= turn_time;
		else if(wait_time1 <= 0)
			set_STATE(goback);
	}

	if(STATE == goback)
	{
		wait_time1 = def_wait_time1;
		if(collision_top || current_position->y <= start_y)
			set_STATE(waiting1);
		else
			y_velocity -= MOVINGBLOCK3_V_VELOCITY;
	}
}

void GameObject_Movingwall3::set_STATE(ObjectState state, bool final)
{
	if(state == active || state == waiting2)
		set_type(kill_all);
	else
		set_type(wall_elevator);


	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Movingwall3::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/movingwall3/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Movingwall3::turn(int time_elapsed)
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

void GameObject_Movingwall3::show(int view_x, int view_y)
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
