/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_map
#define header_map

#if _MSC_VER > 1000
#pragma once
#endif

#include "precomp.h"
#include "tilemap.h"
#include "re_resourcemanager.h"
#include "re_gamecontext.h"
#include "re_point.h"
#include "re_rect.h"

class Location
{
public:
	int x;
	int y;
	int state;
	int scroll_x;
	int scroll_y;
};

class Connection
{
public:
	RE_Rect* box;
	int map;
	int submap;
	int location;
};

class Map : public TileMap
{
//!Enums:
public:
	//: Different tile types in the game.
	enum TileType
	{
		tile_empty = 0,   // Tile is empty.
		tile_platform,    // Tile holds is a platform.
		tile_stairs,      // Tile holds a stair.
		tile_top_stairs,  // tile is the top of a stair
		tile_wall         // Tile is a wall.
	};

//!Construction:
public:
	//:  Constructor. Loads the resources needed to display the map.
	Map(RE_ResourceManager *map_resources, RE_GameContext* gc);

	//: Destructor.
	~Map();

//!Attributes:
public:
	//: Get the tile type of the tile at position (x,y).
	TileType get_tile_type(int x, int y);
	TileType get_tile_type(RE_Point *point);
	int get_map_width(); //map width in pixels
	int get_map_height(); //map height in pixels

	int get_player_x(int location) { return locations[location].x; }
	int get_player_y(int location) { return locations[location].y; }
	int get_player_state(int location) { return locations[location].state; }
	int get_scroll_x(int location) { return locations[location].scroll_x; }
	int get_scroll_y(int location) { return locations[location].scroll_y; }

	int check_connection_collision(RE_Rect* player_cb);

	int get_total_objects() { return total_objects; }
	int get_map(int connection) { return connections[connection].map; }
	int get_submap(int connection) { return connections[connection].submap; }
	int get_location(int connection) { return connections[connection].location; }


//!Operations:
public:
	//: Generate a level of the specified size.
	//: The num_connections specify the amount of connections between the
	//: tunnels in the maze.
	void load_level(RE_GameContext* gc, RE_ResourceManager *map_resources);


//!Implementation:
private:
	//: Set the tile to the specified type.
	void set_tile_type(int x, int y, TileType type);

	//map (tile type).
	std::vector<TileType> tiles;

	std::vector<Connection> connections;
	std::vector<Location> locations;

	int total_objects;

};

#endif
