/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_roller2
#define header_gameobject_roller2

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_roller.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Roller2 : public GameObject_Roller
{
//!Construction:
public:
	GameObject_Roller2(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Roller2();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Roller2(world, object_element);
	}
};
#endif