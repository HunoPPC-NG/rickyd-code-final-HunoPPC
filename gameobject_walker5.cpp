/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_walker5.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// construction:

GameObject_Walker5::GameObject_Walker5(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Walker(world, object_element, "walker5")
{

}

GameObject_Walker5::~GameObject_Walker5()
{

}

