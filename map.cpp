/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "map.h"
#include "tinyxml2.h"

//
// Map construction:

Map::Map(RE_ResourceManager* map_resources, RE_GameContext* gc):
 TileMap(gc)
{
	load_level(gc, map_resources);
	//fill tiles
	tiles.reserve(layers[0].map.size());

	for(size_t l = 0; l < layers.size(); ++l)
	{
		for(int current_tile_y = 0; current_tile_y < get_height(); ++current_tile_y)
		{
			for(int current_tile_x = 0; current_tile_x < get_width(); ++current_tile_x)
			{
				int index = current_tile_y * get_width() + current_tile_x;

				if(layers[l].name == "walls")
				{
					switch(layers[l].map[index])
					{
						case 0:
							tiles[index]=tile_empty;
							break;
						default:
							tiles[index]=tile_wall;
							break;
					}
				}
				else if(layers[l].name == "platform")
				{
					switch(layers[l].map[index])
					{
						case 0:
							break;
						default:
							tiles[index]=tile_platform;
							break;
					}
				}
				else if(layers[l].name == "stairs")
				{
					switch(layers[l].map[index])
					{
						case 0:
							break;
						default:
							tiles[index]=tile_stairs;
							break;

					}
				}
			}
		}
	}

//	std::cout << "map size:"<< get_width()*get_height()<<" width:"<< get_width() <<"\n";
	//define tile_top_stairs
	//tile_top_stairs is tile_platform with bellow tile_stairs
	for(size_t i = 0; i < (get_height()-1)*get_width(); i++) //don't check last row
	{
		//std::cout << "checking tile "<< i<<"\n";
		if(tiles[i] == tile_platform)
		{
			//check bellow for stairs
			size_t bellow_tile_index = i+get_width();
			//we don't check the last row so bellow_tile_index is always valid:
			//  bellow_tile_index < tiles.size()
			if(tiles[bellow_tile_index] == tile_stairs)
				tiles[i] = tile_top_stairs;
		}
	}
}

Map::~Map()
{
	for (
		std::vector<Connection>::iterator it = connections.begin();
		it != connections.end();
		it++)
	{
		Connection cur = *it;
		delete(cur.box);
		cur.box=NULL;
	}
	connections.clear();
	locations.clear();
	tiles.clear();
}

//
// Map attributes:

Map::TileType Map::get_tile_type(int x, int y)
{
	int mx = abs(x / get_tile_width());
	int my = abs(y / get_tile_height());

	return(tiles[mx + my*get_width()]);
}

Map::TileType Map::get_tile_type(RE_Point *point)
{
	int mx = abs(point->x / get_tile_width());
	int my = abs(point->y / get_tile_height());

	return(tiles[mx + my*get_width()]);
}

//
// Map operations:

void Map::load_level(RE_GameContext* gc, RE_ResourceManager* map_resources)
{
	// Clean up any possible earlier map:

	//load new level map
//	std::cout << "Map: loading map resources\n";
	load(map_resources);

	tinyxml2::XMLElement* player_element = map_resources->getElement("map/submap/player");
	tinyxml2::XMLElement* connections_element = map_resources->getElement("map/submap/connections");

	if(connections_element == NULL)
	{
		std::cout << "Map: no connections element in map file\n";
		exit(1);
	}
	if(player_element == NULL)
	{
		std::cout << "Map: no player element in map file\n";
		exit(1);
	}
	//query connections
//	  std::cout << "Querying map connections\n";
	tinyxml2::XMLElement* conn_element = NULL;
	for(conn_element = connections_element->FirstChildElement("position"); conn_element != NULL; conn_element = conn_element->NextSiblingElement("position"))
	{
		//position 0 is default start position
		Connection conn;

		int y = 0;
		int heigth = 0;
		int x = 0;
		int width = 0;
		conn.map = 0;
		conn.submap = 0;
		conn.location= 0;

		conn_element->QueryIntAttribute("map", &conn.map);
		conn_element->QueryIntAttribute("submap", &conn.submap);
		conn_element->QueryIntAttribute("location", &conn.location);

		conn_element->QueryIntAttribute("x", &x);
		conn_element->QueryIntAttribute("y", &y);
		conn_element->QueryIntAttribute("height", &heigth);
		conn_element->QueryIntAttribute("width", &width);

		conn.box = new RE_Rect(x, y-heigth, x+width, y);

		connections.push_back(conn);
	}

	//query Locations
//	  std::cout << "Querying player start locations\n";
	tinyxml2::XMLElement* loc_element = NULL;
	for(loc_element = player_element->FirstChildElement("position"); loc_element != NULL; loc_element = loc_element->NextSiblingElement("position"))
	{
		//position 0 is default start position
		Location loc;

		loc.x=0;
		loc.y=0;
		loc.state=0;
		loc.scroll_x=0;
		loc.scroll_y=0;

		loc_element->QueryIntAttribute("x", &loc.x);
		loc_element->QueryIntAttribute("y", &loc.y);
		loc_element->QueryIntAttribute("state", &loc.state);
		loc_element->QueryIntAttribute("scroll_x", &loc.scroll_x);
		loc_element->QueryIntAttribute("scroll_y", &loc.scroll_y);

		locations.push_back(loc);
	}

	total_objects = 0;//FIXME: no need for this

}


//
// Map implementation:

void Map::set_tile_type(int x, int y, TileType type)
{
	tiles[x + y*get_width()] = type;
}

int Map::get_map_width()
{
	return get_width() * get_tile_width();
}

int Map::get_map_height()
{
	return get_height() * get_tile_height();
}

int Map::check_connection_collision(RE_Rect* player_cb)
{
	int index = 0;
	for (
		std::vector<Connection>::iterator it = connections.begin();
		it != connections.end();
		it++)
	{
		Connection cur = *it;
		if(player_cb->is_overlapped(cur.box))
			return index;

		index++;
	}

	return -1;
}
