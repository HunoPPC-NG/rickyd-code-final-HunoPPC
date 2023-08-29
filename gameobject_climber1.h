/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_climber1
#define header_gameobject_climber1

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_climber.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Climber1 : public GameObject_Climber
{
//!Construction:
public:
	GameObject_Climber1(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Climber1();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Climber1(world, object_element);
	}
};

#endif
