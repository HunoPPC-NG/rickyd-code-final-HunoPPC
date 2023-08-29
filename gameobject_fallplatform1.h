/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_fallplatform1
#define header_gameobject_fallplatform1

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_fallplatform.h"
#include "re_font.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Fallplatform1 : public GameObject_Fallplatform
{
//!Construction:
public:
	GameObject_Fallplatform1(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Fallplatform1();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Fallplatform1(world, object_element);
	}
};

#endif
