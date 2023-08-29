/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_vprojectile1
#define header_gameobject_vprojectile1

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_vprojectile.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Vprojectile1 : public GameObject_Vprojectile
{
//!Construction:
public:
	GameObject_Vprojectile1(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Vprojectile1();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Vprojectile1(world, object_element);
	}
};
#endif