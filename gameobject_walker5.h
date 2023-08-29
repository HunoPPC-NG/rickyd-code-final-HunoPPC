/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_walker5
#define header_gameobject_walker5

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_walker.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Walker5 : public GameObject_Walker
{
//!Construction:
public:
	GameObject_Walker5(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Walker5();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Walker5(world, object_element);
	}
};

#endif