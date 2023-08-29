/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_walker2.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Walker2 construction:

GameObject_Walker2::GameObject_Walker2(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Walker(world, object_element, "walker2")
{

}

GameObject_Walker2::~GameObject_Walker2()
{

}
