/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <ctime>
#include "precomp.h"
#include "gameobject_bonus1.h"
#include "world.h"

GameObject_Bonus1::GameObject_Bonus1(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Bonus(world, object_element, "bonus1")
{

}

GameObject_Bonus1::~GameObject_Bonus1()
{

}