/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_jumpplatform.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_JumpPlatform construction:

GameObject_JumpPlatform::GameObject_JumpPlatform(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(jump_platform);

	//spr_object[active] = new RE_Sprite(world->gc, "global/objects/jumpplatform/active/waiting/sprite", world->global_resources);

	//sfx_jumpplatform = new RE_SoundBuffer("global/objects/jumpplatform/sounds/grab/sample", world->global_resources);
	//sfx_jumpplatform->set_volume(1.0f);

	//sound = new RE_SoundSession();

	platform_width = 60;
	platform_height = 20;
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("width", &platform_width);
	object_element->FirstChildElement("position")->QueryIntAttribute("height", &platform_height);

	set_STATE(active);
	update_collision_box();

}

GameObject_JumpPlatform::~GameObject_JumpPlatform()
{
	//delete(sound);
	//delete(sfx_jumpplatform);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_JumpPlatform attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_JumpPlatform operations:

void GameObject_JumpPlatform::update_STATE()
{
	if(STATE == inactive)
		return;

}


void GameObject_JumpPlatform::set_STATE(ObjectState state)
{
	previous_STATE = STATE;
	STATE = state;
}


void GameObject_JumpPlatform::update_collision_box()
{
	//tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/jumpplatform/active/collision_box");
	CB.width = platform_width;
    CB.height = platform_height;
    CB.dx = 0;
    CB.dy = 0;
}

bool GameObject_JumpPlatform::turn(int time_elapsed)
{
	update_STATE();

	return true;
}

void GameObject_JumpPlatform::show(int view_x, int view_y)
{
	return;
}

