/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_statickiller.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_StaticKiller construction:

GameObject_StaticKiller::GameObject_StaticKiller(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	set_type(kill_all);
	set_use_elevator(false);
	can_move = false;
	obj_name = object_name;

	spr_object[right] = NULL;
	spr_object[left] = NULL;
	spr_object[up] = NULL;
	spr_object[down] = NULL;

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/vertical/right/sprite"))
		spr_object[right] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/vertical/right/sprite", world->global_resources);
	if(world->global_resources->file_exists("global/objects/"+obj_name+"/vertical/left/sprite"))
		spr_object[left]  = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/vertical/left/sprite", world->global_resources);
	if(world->global_resources->file_exists("global/objects/"+obj_name+"/horizontal/up/sprite"))
		spr_object[up]    = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/horizontal/up/sprite", world->global_resources);
	if(world->global_resources->file_exists("global/objects/"+obj_name+"/horizontal/down/sprite"))
		spr_object[down]  = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/horizontal/down/sprite", world->global_resources);

	int initial_state = 2; //up by default
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &initial_state);
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");


	if(initial_state == 0 && !spr_object[right])
	{
		std::cerr<< "GameObject_StaticKiller: (FATAL) direction is set to right but no matching sprite found\n";
		exit(-1);
	}

	if(initial_state == 1 && !spr_object[left])
	{
		std::cerr<< "GameObject_StaticKiller: (FATAL) direction is set to left but no matching sprite found\n";
		exit(-1);
	}

	if(initial_state == 2 && !spr_object[up])
	{
		std::cerr<< "GameObject_StaticKiller: (FATAL) direction is set to up but no matching sprite found\n";
		exit(-1);
	}

	if(initial_state == 3 && !spr_object[down])
	{
		std::cerr<< "GameObject_StaticKiller: (FATAL) direction is set to down but no matching sprite found\n";
		exit(-1);
	}

	switch(initial_state)
	{
		case 0:
			set_STATE(right);
			break;
		case 1:
			set_STATE(left);
			break;
		case 2:
			set_STATE(up);
			break;
		case 3:
			set_STATE(down);
			break;
	}
	update_collision_box();


	//check for parameters
	anim_pos = 0;
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("anim_pos", &anim_pos);
	}

	x_velocity = 0;
	y_velocity = 0;

	turn_time = 0;
	set_speed(4); //animation speed in frames per sec

}

GameObject_StaticKiller::~GameObject_StaticKiller()
{
	if(spr_object[right])
		delete (spr_object[right]);
	if(spr_object[left])
		delete (spr_object[left]);
	if(spr_object[up])
		delete (spr_object[up]);
	if(spr_object[down])
		delete (spr_object[down]);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_StaticKiller attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_StaticKiller operations:

bool GameObject_StaticKiller::in_window(ObjectState state)
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

void GameObject_StaticKiller::update_STATE()
{
	if(STATE == inactive) //never happens with SaticKiller
		return;
}

void GameObject_StaticKiller::set_STATE(ObjectState state)
{

	previous_STATE = STATE;
	STATE = state;
}


void GameObject_StaticKiller::update_collision_box()
{
	std::string action;

	switch(STATE)
	{
		case left:
		case right:
			action = "vertical";
			break;
		case up:
		case down:
			action = "horizontal";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_StaticKiller::turn(int time_elapsed)
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

void GameObject_StaticKiller::show(int view_x, int view_y)
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
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}

