/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "tilemap.h"
#include "tinyxml2.h"

TileMap::TileMap(RE_GameContext* pgc)
{
	gc = pgc;
}

TileMap::~TileMap()
{
	layers.clear();
	delete(sprite_tiles);
}

void TileMap::load(RE_ResourceManager* map_resource)
{	
	tinyxml2::XMLElement* map_element = map_resource->getElement("map");
	
	std::string level_name = map_element->Attribute("name");
	int map_num = map_element->IntAttribute("index");
	
	tinyxml2::XMLElement* submap_element = map_resource->getElement("map/submap");
	int submap_num = submap_element->IntAttribute("index");

	sprite_tiles = new RE_Sprite(gc, "map/submap/tileset/sprite", map_resource);
	
	tile_width = sprite_tiles->get_width();
	tile_height = sprite_tiles->get_height();
	
	tinyxml2::XMLElement* tilemap_element = map_resource->getElement("map/submap/tilemap");

	map_width = tilemap_element->IntAttribute("width");
	map_height = tilemap_element->IntAttribute("height");
	
	//load layers
	for(tinyxml2::XMLElement* layer_elem = tilemap_element->FirstChildElement( "layer" ); layer_elem != NULL; layer_elem = layer_elem->NextSiblingElement("layer"))
	{
		TileMapLayer layer;
		layer.name = layer_elem->Attribute("name");
		
		std::string layer_tiles = layer_elem->GetText();
	
		RE_Helpers helper;
		std::vector<std::string> tile_indices = helper.split(layer_tiles, ',');
		
		layer.map.reserve(tile_indices.size());
		for(size_t i = 0; i < tile_indices.size(); ++i)
			layer.map.push_back(helper.text_to_int(tile_indices[i]));
	
		layers.push_back(layer);
	}

	current_map_position_x = 0;
	current_map_position_y = 0;
}

int TileMap::get_layer_ind(std::string layer_name)
{
	//get layer index
	for(int l = 0; l < layers.size(); ++l)
	{
		if(layers[l].name == layer_name)
			return l;
	}
	
	return -1;
}

void TileMap::draw(std::string layer_name)
{
	int screen_width = gc->get_width();
	int screen_height = gc->get_height();

	int start_tile_x = floor(current_map_position_x / tile_width); 
	int start_tile_y = floor(current_map_position_y / tile_height); 

	int scrolling_pixel_offset_x = current_map_position_x - start_tile_x * tile_width;
	int scrolling_pixel_offset_y = current_map_position_y - start_tile_y * tile_height;

	int tiles_on_screen_horizontally = screen_width / tile_width + 1;
	int tiles_on_screen_vertically = screen_height / tile_height + 1; 

	// since we might show half tiles on edges, make sure we display one more tile to fill screen
	tiles_on_screen_horizontally++;
	tiles_on_screen_vertically++;

	// make sure we don't draw more than the level size
	if(tiles_on_screen_horizontally + start_tile_x > map_width)
		tiles_on_screen_horizontally = map_width - start_tile_x;
	if(tiles_on_screen_vertically + start_tile_y > map_height)
		tiles_on_screen_vertically = map_height - start_tile_y;
	
	int layer_ind = get_layer_ind(layer_name);
	
	if(layer_ind < 0)
		return;
		
	for(int current_tile_y = 0; current_tile_y < tiles_on_screen_vertically; ++current_tile_y)
	{
		for(int current_tile_x = 0; current_tile_x < tiles_on_screen_horizontally; ++current_tile_x)
		{
			int index = (start_tile_y + current_tile_y) * map_width + (start_tile_x + current_tile_x);
			int tile_position_screen_x = current_tile_x * tile_width - scrolling_pixel_offset_x;
			int tile_position_screen_y = current_tile_y * tile_height - scrolling_pixel_offset_y;
				
			if(layers[layer_ind].map[index] >  0) //don't draw tiles with value 0 (empty)
			{
				int sprite_index = layers[layer_ind].map[index] - 1;
				
				sprite_tiles->set_frame(sprite_index);
				sprite_tiles->draw(tile_position_screen_x, tile_position_screen_y);
			} 
		}
	}
		
}

void TileMap::draw()
{
	int screen_width = gc->get_width();
	int screen_height = gc->get_height();

	int start_tile_x = floor(current_map_position_x / tile_width); 
	int start_tile_y = floor(current_map_position_y / tile_height); 

	int scrolling_pixel_offset_x = current_map_position_x - start_tile_x * tile_width;
	int scrolling_pixel_offset_y = current_map_position_y - start_tile_y * tile_height;

	int tiles_on_screen_horizontally = screen_width / tile_width + 1;
	int tiles_on_screen_vertically = screen_height / tile_height + 1; 

	// since we might show half tiles on edges, make sure we display one more tile to fill screen
	tiles_on_screen_horizontally++;
	tiles_on_screen_vertically++;

	// make sure we don't draw more than the level size
	if(tiles_on_screen_horizontally + start_tile_x > map_width)
		tiles_on_screen_horizontally = map_width - start_tile_x;
	if(tiles_on_screen_vertically + start_tile_y > map_height)
		tiles_on_screen_vertically = map_height - start_tile_y;

	for(size_t l = 0; l < layers.size(); ++l)
	{
		for(int current_tile_y = 0; current_tile_y < tiles_on_screen_vertically; ++current_tile_y)
		{
			for(int current_tile_x = 0; current_tile_x < tiles_on_screen_horizontally; ++current_tile_x)
			{
				int index = (start_tile_y + current_tile_y) * map_width + (start_tile_x + current_tile_x);
				int tile_position_screen_x = current_tile_x * tile_width - scrolling_pixel_offset_x;
				int tile_position_screen_y = current_tile_y * tile_height - scrolling_pixel_offset_y;
				
				if(layers[l].map[index] >  0) //don't draw tiles with value 0 (empty)
				{
					int sprite_index = layers[l].map[index] - 1;

					sprite_tiles->set_frame(sprite_index);
					sprite_tiles->draw(tile_position_screen_x, tile_position_screen_y);
				} 
			}
		}
	}
}

void TileMap::set_scroll(int x, int y)
{
	current_map_position_x = x;
	current_map_position_y = y;
}
