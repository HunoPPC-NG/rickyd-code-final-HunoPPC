/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_bonus1
#define header_gameobject_bonus1

#if _MSC_VER > 1000
#pragma once
#endif


#include "precomp.h"
#include "gameobject_bonus.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Bonus1 : public GameObject_Bonus
{
//!Construction:
public:
	GameObject_Bonus1(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Bonus1();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Bonus1(world, object_element);
	}
};

#endif
