/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <ctime>
#include "precomp.h"
#include "gameobject_bonus3.h"
#include "world.h"

GameObject_Bonus3::GameObject_Bonus3(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Bonus(world, object_element, "bonus3")
{

}

GameObject_Bonus3::~GameObject_Bonus3()
{

}
