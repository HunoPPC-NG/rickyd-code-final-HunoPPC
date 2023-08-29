/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <ctime>
#include "precomp.h"
#include "gameobject_bonus2.h"
#include "world.h"

GameObject_Bonus2::GameObject_Bonus2(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Bonus(world, object_element, "bonus2")
{

}

GameObject_Bonus2::~GameObject_Bonus2()
{

}