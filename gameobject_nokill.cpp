/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_nokill.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Nokill construction:

GameObject_Nokill::GameObject_Nokill(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	spr_object[walk_right]  = new RE_Sprite(world->gc, "global/objects/nokill/walk/right/sprite", world->global_resources);
	spr_object[walk_left] = new RE_Sprite(world->gc, "global/objects/nokill/walk/left/sprite", world->global_resources);
	spr_object[resting]  = new RE_Sprite(world->gc, "global/objects/nokill/resting/sprite", world->global_resources);

	anim_pos = 0;

	i_am_dead = false;

	set_speed(4); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	direction = object_element->FirstChildElement("position")->IntAttribute("state");

	set_STATE(resting);
	update_collision_box();

	TriggerBox = NULL;
	bounding_box = NULL;

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

	//check for bounding_box
	tinyxml2::XMLElement* bound_element = NULL;
	bound_element = object_element->FirstChildElement("bounding_box");
	if(bound_element)
	{
		int bb_x = bound_element->IntAttribute("x");
		int bb_y = bound_element->IntAttribute("y");
		int bb_width = bound_element->IntAttribute("width");
		int bb_height = bound_element->IntAttribute("height");

		set_bounding_box(bb_x, bb_y, bb_width, bb_height);
	}

	def_walk_time = NOKILL_WALK_TIME;
	def_rest_time = NOKILL_REST_TIME;
	default_speed1 = 5;
	default_speed2 = 10;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("walk_time", &def_walk_time);
		param_element->QueryIntAttribute("rest_time", &def_rest_time);
		param_element->QueryIntAttribute("anim_velocity1", &default_speed1);
		param_element->QueryIntAttribute("anim_velocity2", &default_speed2);

	}
	rest_time = def_rest_time;
	walk_time = def_walk_time;

	turn_time = 0;
	move_time = 0;
}

GameObject_Nokill::~GameObject_Nokill()
{
	delete(spr_object[walk_left]);
	delete(spr_object[walk_right]);
	delete(spr_object[resting]);

	if(TriggerBox)
		delete(TriggerBox);
	if(bounding_box)
		delete(bounding_box);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Nokill attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Nokill operations:
void GameObject_Nokill::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Nokill::check_trigger()
{
	if(!TriggerBox)
		return true;

	if (world->bullet->get_type() == kill_all &&
	         check_box_collision(world->bullet->get_collision_rect()))
	{	//bullet collision
		world->bullet->set_object_collision(this);
		return true;
	}
	else if (world->bomb->get_type() == kill_all &&
			 check_box_collision(world->bomb->get_collision_rect()))
	{	//bomb collision
		world->bomb->set_object_collision(this);
		return true;
	}

	//check player collision
	if(world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}

void GameObject_Nokill::set_bounding_box(int bb_x, int bb_y, int bb_width, int bb_height)
{
	bounding_box = new RE_Rect(bb_x, bb_y - bb_height, bb_x + bb_width, bb_y);
}

bool GameObject_Nokill::in_window(ObjectState state)
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

void GameObject_Nokill::update_STATE()
{
	if(STATE == inactive)
		return;

	if((STATE == resting) &&
		check_trigger() &&
		(rest_time <= 0))
	{
		if(direction > 0)
			set_STATE(walk_right);
		else
			set_STATE(walk_left);
	}
	else if ( STATE == resting &&
		      rest_time > 0)
	{
		rest_time -= turn_time;
	}

	if(STATE == walk_right)
	{
		if((current_position->x - world->scroll_x + get_width() + 1) > world->gc->get_width() - world->map->get_tile_width())
			set_STATE(inactive);
		else if(collision_right ||
			current_position->x + spr_object[STATE]->get_width() > bounding_box->Right())
			set_STATE(walk_left);
	}
	else if (STATE == walk_left)
	{
		if((current_position->x - world->scroll_x - 1) < world->map->get_tile_width())
			set_STATE(inactive);
		else if(collision_left ||
			current_position->x < bounding_box->Left())
			set_STATE(walk_right);
	}

	if( def_rest_time > 0 &&
	   ( STATE == walk_right || STATE == walk_left ))
	{
		if(walk_time > 0)
			walk_time -= turn_time;
		else
			set_STATE(resting);
	}

	y_velocity += GRAVITY;
}

void GameObject_Nokill::deploy(int start_x, int start_y, int state)
{
	if(state >= 0)
	{
		current_position->x = start_x;
		current_position->y = start_y;
		direction = state;
	}
	else
	{
		current_position->x += start_x;
		current_position->y += start_y;
		direction = state;
	}
}

void GameObject_Nokill::set_STATE(ObjectState state, bool final)
{
	switch(state)
	{
		case inactive:
			set_type(kill_none);
			break;

		case walk_right:
			x_velocity = NOKILL_VELOCITY;
			anim_pos = 0;
			set_type(kill_all);
			direction = 1;
			set_speed(default_speed1);
			break;

		case walk_left:
			x_velocity = -NOKILL_VELOCITY;
			anim_pos = 0;
			set_type(kill_all);
			direction = -1;
			set_speed(default_speed1);
			break;

		case resting:
			x_velocity = 0;
			anim_pos = 0;
			set_type(kill_all);
			rest_time = def_rest_time;
			walk_time = def_walk_time;
			set_speed(default_speed2);
			break;
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;

	update_collision_box();
}

void GameObject_Nokill::update_collision_box()
{
	std::string action;

	switch(STATE)
	{
		case walk_right:
		case walk_left:
			action = "walk";
			break;
		default:
			action = "resting";
			break;
	}
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/nokill/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Nokill::turn(int time_elapsed)
{
	if(STATE == inactive)
		return false;

	turn_time = time_elapsed;

	update_STATE();

	bool ret = true;

	ret = move(time_elapsed);

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;
	//update_collision_box();

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	return ret;
}

void GameObject_Nokill::show(int view_x, int view_y)
{
	if(STATE == inactive)
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
