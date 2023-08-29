/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_bombpack.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_Mask1bonus construction:

GameObject_Bombpack::GameObject_Bombpack(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(bonus_points);

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/bombpack/active/waiting/sprite", world->global_resources);

	sfx_bombpack = new RE_SoundBuffer("global/objects/bombpack/sounds/grab/sample", world->global_resources);
	sfx_bombpack->set_volume(1.0f);

	sound = new RE_SoundSession();

	total_bombs = 6;
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("total", &total_bombs);

	set_STATE(active);
	update_collision_box();

	anim_pos = 0;
	turn_time = 0;
	set_speed(4); //animation speed in frames per sec

}

GameObject_Bombpack::~GameObject_Bombpack()
{
	delete (spr_object[active]);

	delete(sound);
	delete(sfx_bombpack);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Bombpack attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Bombpack operations:

void GameObject_Bombpack::update_STATE()
{
	if(STATE == inactive)
		return;

	if(world->check_collision(player, this))
	{
		set_STATE(inactive);
		sound->play(sfx_bombpack);
		world->bomb->set_total(total_bombs);
	}
	else if (world->bullet->get_type() == kill_all &&
	         check_box_collision(world->bullet->get_collision_rect()))
	{	//bullet collision
		world->bullet->set_object_collision(this);
		set_STATE(inactive);
	}
	else if (world->bomb->get_type() == kill_all &&
			 check_box_collision(world->bomb->get_collision_rect()))
	{	//bomb collision
		world->bomb->set_object_collision(this);
		set_STATE(inactive);
	}

	y_velocity += GRAVITY;
}


void GameObject_Bombpack::set_STATE(ObjectState state)
{
	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Bombpack::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/bombpack/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Bombpack::turn(int time_elapsed)
{
	update_STATE();

	if(STATE == inactive)
		return false;

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	return move(time_elapsed);
}

void GameObject_Bombpack::show(int view_x, int view_y)
{
	if(STATE == inactive)
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

