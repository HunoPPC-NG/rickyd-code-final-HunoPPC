/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_bonus2
#define header_gameobject_bonus2

#if _MSC_VER > 1000
#pragma once
#endif


#include "precomp.h"
#include "gameobject_bonus.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Bonus2 : public GameObject_Bonus
{
//!Construction:
public:
	GameObject_Bonus2(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Bonus2();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Bonus2(world, object_element);
	}
};

#endif