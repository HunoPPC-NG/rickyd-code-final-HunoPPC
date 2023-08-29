/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_statickiller2
#define header_gameobject_statickiller2

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_statickiller.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_StaticKiller2 : public GameObject_StaticKiller
{
//!Construction:
public:
	GameObject_StaticKiller2(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_StaticKiller2();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_StaticKiller2(world, object_element);
	}
};

#endif
