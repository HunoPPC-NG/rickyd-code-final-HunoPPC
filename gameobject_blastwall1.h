/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_blastwall1
#define header_gameobject_blastwall1

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_blastwall.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Blastwall1 : public GameObject_Blastwall
{
//!Construction:
public:
	GameObject_Blastwall1(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Blastwall1();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Blastwall1(world, object_element);
	}
};
#endif