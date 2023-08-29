/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_spikerock.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Spikewall construction:

GameObject_Spikerock::GameObject_Spikerock(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(kill_all);
	set_STATE(active);
	can_move = false;

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/spikerock/active/anim/sprite", world->global_resources);
	spr_object[blasted] = new RE_Sprite(world->gc, "global/objects/spikerock/blasted/anim/sprite", world->global_resources);

	anim_pos = 0;

	i_am_dead = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

	update_collision_box();

	turn_time = 0;

}

GameObject_Spikerock::~GameObject_Spikerock()
{
	delete(spr_object[active]);
	delete(spr_object[blasted]);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Spikerock attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Spikerock operations:
void GameObject_Spikerock::update_STATE()
{
	if(STATE == blasted)
		return;

	x_velocity = 0;
	y_velocity += 0.5;

	if( (world->bomb->get_type() == kill_all) &&
		get_collision_rect()->is_overlapped(world->bomb->get_collision_rect()))
	{
		set_STATE(blasted);
		set_type(kill_none);
	}
}

void GameObject_Spikerock::set_STATE(ObjectState state, bool final)
{
	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Spikerock::update_collision_box()
{
	if(STATE == blasted)
		return;

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/spikerock/active/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Spikerock::turn(int time_elapsed)
{

	update_STATE();

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	//bool ret = move(turn_time, false);

	//update_collision_box();

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	return true;
}

void GameObject_Spikerock::show(int view_x, int view_y)
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
