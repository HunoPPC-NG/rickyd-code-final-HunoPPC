/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_background.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_RedLight construction:

GameObject_Background::GameObject_Background(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	set_type(kill_none);
	set_use_elevator(false);
	can_move = false;
	obj_name = object_name;

	spr_object[active] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/active/sprite", world->global_resources);

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

	anim_pos = 0;
	int fgrd = 0;
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &anim_pos);
	object_element->FirstChildElement("position")->QueryIntAttribute("foreground", &fgrd);
	if (fgrd > 0)
		foreground = true;

	update_collision_box(); //just for initialization of CB

	set_STATE(active);

	move_time = 0;
	set_speed(4); //animation speed in frames per sec

}

GameObject_Background::~GameObject_Background()
{
	delete (spr_object[active]);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_RedLight attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_RedLight operations:

bool GameObject_Background::in_window(ObjectState state)
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

void GameObject_Background::set_STATE(ObjectState state)
{
	STATE = state;
}


void GameObject_Background::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = NULL;
	cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/active/collision_box");
	if(cb_element)
	{
		CB.width = cb_element->IntAttribute("width");
		CB.height = cb_element->IntAttribute("height");
		CB.dx = cb_element->IntAttribute("dx");
		CB.dy = cb_element->IntAttribute("dy");
	}
	else
	{	//background objects have no collisions by default (just set some value)
		CB.width = 10;
		CB.height = 10;
		CB.dx = 0;
		CB.dy = 0;
	}
}

bool GameObject_Background::turn(int time_elapsed)
{
	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	//update_collision_box();

	if(move_time == 0) //rotate on turn start
		anim_pos++;

}

void GameObject_Background::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	//only render if in window
	if(!in_window(STATE))
		return;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[active]->get_height() +1) - view_y);
}

