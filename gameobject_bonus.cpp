/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_bonus.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_Mask1bonus construction:

GameObject_Bonus::GameObject_Bonus(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	set_type(bonus_points);
	obj_name = object_name;

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/active/waiting/sprite", world->global_resources);

	sfx_bonus = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/grab/sample", world->global_resources);
	sfx_bonus->set_volume(1.0f);

	sound = new RE_SoundSession();

	bonus_value = BONUS_VALUE;
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("points", &bonus_value);

	std::ostringstream scoreSStream;
	scoreSStream << bonus_value;
	std::string score_text = scoreSStream.str();

	score_font = new RE_Font(world->gc, "global/objects/"+obj_name+"/font", world->global_resources);
	score_font->set_text(score_text);

	set_STATE(active);
	update_collision_box();

	anim_pos = 0;
	turn_time = 0;
	move_time = 0;
	score_time = BONUS_SCORE_TIME;
	set_speed(4); //animation speed in frames per sec

	world->add_bonus(get_id());

	if(!world->check_bonus(get_id()))
		set_STATE(inactive);
}

GameObject_Bonus::~GameObject_Bonus()
{
	delete (spr_object[active]);
	delete (score_font);

	delete(sound);
	delete(sfx_bonus);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Bonus attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Bonus operations:

bool GameObject_Bonus::in_window(ObjectState state)
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

void GameObject_Bonus::update_STATE()
{
	if(STATE == inactive)
		return;

	if(STATE == captured && score_time > 0)
	{
		score_time -=turn_time;
		return;
	}
	else if (STATE == captured && score_time <= 0)
	{
		set_STATE(inactive);
		return;
	}

	if(world->check_collision(player, this))
	{
		set_STATE(captured);
		sound->play(sfx_bonus);
		world->add_score(bonus_value);
		world->disable_bonus(get_id());
	}
	else if ( world->bomb->get_type() == kill_all )
	{
		//check collision with bomb explosion
		if(check_box_collision(world->bomb->get_collision_rect()))
		{
			world->bomb->set_object_collision(this);
			set_STATE(inactive);
			world->disable_bonus(get_id());
		}
	}
	else if ( world->bullet->get_type() == kill_all )
	{
		//check collision with firing bullet
		if(check_box_collision(world->bullet->get_collision_rect()))
		{
			world->bullet->set_object_collision(this);
			set_STATE(inactive);
			world->disable_bonus(get_id());
		}
	}

}


void GameObject_Bonus::set_STATE(ObjectState state)
{
	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Bonus::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Bonus::turn(int time_elapsed)
{
	update_STATE();

	if(STATE == inactive)
		return false;

	turn_time = time_elapsed;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(STATE == captured)
	{
		y_velocity = -2;
		return move(time_elapsed, false);
	}
	else
		y_velocity += GRAVITY;

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	return move(time_elapsed);
}

void GameObject_Bonus::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	if(STATE == captured)
	{
		score_font->draw(
			current_position->x  - view_x,
			(current_position->y - spr_object[active]->get_height() +1) - view_y);
		return;
	}

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

