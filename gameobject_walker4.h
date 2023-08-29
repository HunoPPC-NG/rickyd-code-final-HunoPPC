/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_walker4
#define header_gameobject_walker4

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_walker.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Walker4 : public GameObject_Walker
{
//!Construction:
public:
	GameObject_Walker4(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Walker4();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Walker4(world, object_element);
	}
};

#endif
