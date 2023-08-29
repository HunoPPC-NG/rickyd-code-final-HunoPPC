/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "re_tmxconverter.h"

RE_TmxConverter::RE_TmxConverter(std::string tmx_file, std::string m_file)
{
	doc = new tinyxml2::XMLDocument();
	if( doc->LoadFile(tmx_file.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		std::cerr << "RE_TmxConverter: couldn't open \""<< tmx_file << "\" for read:" << doc->ErrorID() <<"\n";
		exit(1);
	}
	map_elem = doc->FirstChildElement( "map" );
	tileset_elem = map_elem->FirstChildElement( "tileset" );

	map_file = m_file;
}

RE_TmxConverter::~RE_TmxConverter()
{
	delete(doc);
}

void RE_TmxConverter::convert()
{
	int map_width  = map_elem->IntAttribute("width");
	int map_height = map_elem->IntAttribute("height");

	int tile_width  = tileset_elem->IntAttribute("tilewidth");
	int tile_height = tileset_elem->IntAttribute("tileheight");

	std::string tileset_file = tileset_elem->FirstChildElement( "image" )->Attribute("source");
	int tileset_width = tileset_elem->FirstChildElement( "image" )->IntAttribute("width");
	int tileset_height = tileset_elem->FirstChildElement( "image" )->IntAttribute("height");

	tinyxml2::XMLDocument* outdoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLNode* outres_node = outdoc->InsertEndChild( outdoc->NewElement( "resources" ) );
	tinyxml2::XMLNode* outmap_node = outres_node->InsertEndChild( outdoc->NewElement( "map" ) );
	tinyxml2::XMLNode* outsubmap_node = outmap_node->InsertEndChild( outdoc->NewElement( "submap" ) );
	tinyxml2::XMLNode* outtileset_node = outsubmap_node->InsertEndChild( outdoc->NewElement( "tileset" ) );

	tinyxml2::XMLElement* outtilemap_element = outdoc->NewElement( "tilemap" );
	outtilemap_element->SetAttribute("width", map_width);
	outtilemap_element->SetAttribute("height", map_height);

	tinyxml2::XMLElement* outsprite_element = outdoc->NewElement( "sprite" );
	outsprite_element->SetAttribute( "file", tileset_file.c_str() );

	tinyxml2::XMLNode* outsprite_node = outtileset_node->InsertEndChild( outsprite_element );

	tinyxml2::XMLElement* outgrid_element = outdoc->NewElement( "grid" );
	outgrid_element->SetAttribute( "width", tile_width );
	outgrid_element->SetAttribute( "height", tile_height );
	outgrid_element->SetAttribute( "cols", (int) floor(tileset_width/tile_width) );
	outgrid_element->SetAttribute( "rows", (int) floor(tileset_height/tile_height) );

	outsprite_node->InsertEndChild( outgrid_element );


	tinyxml2::XMLElement* layer_elem = NULL;
	for(layer_elem = map_elem->FirstChildElement("layer"); layer_elem != NULL; layer_elem = layer_elem->NextSiblingElement("layer"))
	{
		std::string layer_name = layer_elem->Attribute("name");
		std::string layer_indexes = "\n";
		int cols = 0;
		int index = map_width * map_height;
		tinyxml2::XMLElement* data_elem = layer_elem->FirstChildElement("data");
		tinyxml2::XMLElement* tile_elem = data_elem->FirstChildElement("tile");
		for(tile_elem = data_elem->FirstChildElement("tile"); tile_elem != NULL; tile_elem = tile_elem->NextSiblingElement("tile"))
		{
			cols++;
			index--;
			std::string tile_index = tile_elem->Attribute("gid");
			layer_indexes += tile_index;
			if(index > 0)
				layer_indexes += ", ";
			if(cols >= map_width)
			{
				layer_indexes += "\n";
				cols = 0;
			}
		}

		tinyxml2::XMLElement* outlayer_elem = outdoc->NewElement( "layer" );
		outlayer_elem->SetAttribute( "name", layer_name.c_str() );
		outlayer_elem->InsertFirstChild(outdoc->NewText(layer_indexes.c_str()));

		outtilemap_element->InsertEndChild(outlayer_elem);
	}
	outsubmap_node->InsertEndChild(outtilemap_element);

	tinyxml2::XMLElement* outobjects_elem = outdoc->NewElement( "objects" );
	outobjects_elem->InsertFirstChild(outdoc->NewText("\n"));
	outsubmap_node->InsertEndChild(outobjects_elem);

	outdoc->SaveFile(map_file.c_str());
	delete(outdoc);
}