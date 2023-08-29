/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_walker1.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Walker1 construction:

GameObject_Walker1::GameObject_Walker1(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Walker(world, object_element, "walker1")
{

}

GameObject_Walker1::~GameObject_Walker1()
{

}

