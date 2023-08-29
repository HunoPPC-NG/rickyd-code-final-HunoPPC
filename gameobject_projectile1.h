/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_projectile1
#define header_gameobject_projectile1

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_projectile.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Projectile1 : public GameObject_Projectile
{
//!Construction:
public:
	GameObject_Projectile1(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Projectile1();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Projectile1(world, object_element);
	}
};
#endif
