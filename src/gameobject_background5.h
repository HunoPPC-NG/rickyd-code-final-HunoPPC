/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_background5
#define header_gameobject_background5

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject_background.h"
#include "re_font.h"
#include "re_soundbuffer.h"
#include "re_soundsession.h"
#include "tinyxml2.h"
#include "world.h"

class GameObject_Background5 : public GameObject_Background
{
//!Construction:
public:
	GameObject_Background5(World *world, tinyxml2::XMLElement* object_element);
	~GameObject_Background5();
	static GameObject * __stdcall Create(World *world, tinyxml2::XMLElement* object_element)
	{
		return new GameObject_Background5(world, object_element);
	}
};

#endif
