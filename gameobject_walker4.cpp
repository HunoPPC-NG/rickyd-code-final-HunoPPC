/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_walker4.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Soldier1 construction:

GameObject_Walker4::GameObject_Walker4(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Walker(world, object_element, "walker4")
{

}

GameObject_Walker4::~GameObject_Walker4()
{

}

