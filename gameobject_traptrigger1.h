/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_traptrigger1
#define header_gameobject_traptrigger1

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_traptrigger.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Traptrigger1 : public GameObject_Traptrigger
{
//!Construction:
public:
	GameObject_Traptrigger1(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Traptrigger1();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Traptrigger1(world, object_element);
	}
};
#endif
