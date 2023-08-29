/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "precomp.h"
#include "gameobject_climber3.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// construction:

GameObject_Climber3::GameObject_Climber3(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Climber(world, object_element, "climber3")
{

}

GameObject_Climber3::~GameObject_Climber3()
{

}
