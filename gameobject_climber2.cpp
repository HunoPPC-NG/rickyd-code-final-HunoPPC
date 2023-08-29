/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_climber2.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Climber2 construction:

GameObject_Climber2::GameObject_Climber2(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Climber(world, object_element, "climber2")
{

}

GameObject_Climber2::~GameObject_Climber2()
{

}

