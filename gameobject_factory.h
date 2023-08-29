/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_gameobject_factory
#define header_gameobject_factory

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "gameobject.h"
#include "world.h"
#include "tinyxml2.h"

// Create function pointer
typedef GameObject* (__stdcall *CreateGameObjectFn)(World *world, tinyxml2::XMLElement* object_element);

class GameObjectFactory
{
private:
	GameObjectFactory();
	GameObjectFactory(const GameObjectFactory &) { }
	GameObjectFactory &operator=(const GameObjectFactory &) { return *this; }
 
	typedef map<string, CreateGameObjectFn> FactoryMap;
	FactoryMap m_FactoryMap;
	
public:
	~GameObjectFactory() { m_FactoryMap.clear(); }
 
	static GameObjectFactory *Get()
	{
		static GameObjectFactory instance;
		return &instance;
	}
 
	void Register(const string &objectName, CreateGameObjectFn pfnCreate);
	GameObject *CreateGameObject(const std::string &objectName, World *world, tinyxml2::XMLElement* object_element);
};

#endif
