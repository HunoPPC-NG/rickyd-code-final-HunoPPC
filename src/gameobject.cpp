/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject.h"

//list of existing object ids
static std::list<Uint32> id_list;

//order ids in ascending oder (for id_list.sort)
static bool order_id_asc(Uint32 first, Uint32 second)
{
	bool ret = false;

	if(first < second)
		ret = true;

	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving construction:

GameObject::GameObject()
{
	TYPE = kill_none;
	object_collision = NULL;
	on_climb = false;
	on_elevator = false;
	can_move = true;
	foreground = false;
	set_use_elevator(true); //can use elevator by default

	//initialize to coordenate 0,0
	current_position = new RE_Point();

	Uint32 last_id = 0;
	Uint32 current_id = 0;

	for (
		std::list<Uint32>::iterator iter = id_list.begin();
		iter != id_list.end();
		iter++)
	{
		current_id = *iter;
		if(current_id > last_id + 1) //an empty slot
			break;
		else
			last_id = current_id;
	}

	id = last_id + 1;
	id_list.push_back(id);
	id_list.sort(order_id_asc);
	//std::cout << "created object id "<< id <<"\n";
}

//virtual
GameObject::~GameObject()
{
	delete(current_position);
	for (
		std::list<Uint32>::iterator iter = id_list.begin();
		iter != id_list.end();
		iter++)
	{
		if((*iter) == id )
		{
			id_list.erase(iter);
			break;
		}
	}
}
