/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_tilemap
#define header_tilemap

#include "precomp.h"
#include "re_gamecontext.h"
#include "re_resourcemanager.h"
#include "re_sprite.h"

#pragma once

class TileMapLayer
{
public:
	std::vector<int> map;
	std::string name;
};

class TileMap
{
public:
	TileMap(RE_GameContext* pgc);
	~TileMap();
    //: Get the width of the map in tiles.
	int get_width() { return map_width; }
	//: Get the height of the map in tiles.
	int get_height() { return map_height; }
	//: Get the width of tiles.
	int get_tile_width() { return tile_width; }
	//: Get the height of the tiles.
	int get_tile_height() { return tile_height; }
	
public:
	void load(RE_ResourceManager *map_resource);

	void draw(std::string layer_name); //draw specific layer
	void draw(); //draw all layers
	void set_scroll(int x, int y);

	RE_Sprite* sprite_tiles;
	std::vector<TileMapLayer> layers;

	// current pixel position on map
	int current_map_position_x;
	int current_map_position_y;
	
private:
	RE_GameContext* gc;
	int map_width;
	int map_height;

	int tile_width;
	int tile_height;
	
	int get_layer_ind(std::string layer_name);
};

#endif
