/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_walker3.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Walker3 construction:

GameObject_Walker3::GameObject_Walker3(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Walker(world, object_element, "walker3")
{

}

GameObject_Walker3::~GameObject_Walker3()
{

}
