/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include "re_levelmanager.h"

RE_LevelManager::RE_LevelManager(std::string file, std::string data_dir)
{
	file = data_dir+file;
	num_maps = 0;
	if(file != "")
		load(file);
	else
	{
		std::cerr << "RE_LevelManager: No level file given\n";
	}
}

RE_LevelManager::~RE_LevelManager()
{
}

void RE_LevelManager::load(std::string file)
{
//	  std::cout << "loading " << file << std::endl;

	tinyxml2::XMLDocument doc;

	if( doc.LoadFile(file.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		std::cerr << "RE_LevelManager: couldn't open "<< file << " for read:" << doc.ErrorID() <<"\n";
		return;
	}

	tinyxml2::XMLElement* root_elem = doc.FirstChildElement( "levels" );

	if(!root_elem)
	{
		std::cerr << "RE_LevelManager: no root levels element\n";
		return;
	}



	int num_map = 0;

	for(tinyxml2::XMLElement* map_elem = root_elem->FirstChildElement( "map" ); map_elem != NULL; map_elem = map_elem->NextSiblingElement("map"))
	{
		RE_Map map;

		int value = -1;
		map_elem->QueryIntAttribute( "index", &value );
		map.name = "";
		map.name = map_elem->Attribute("name");
		map.global_file = map_elem->FirstChildElement( "global" )->FirstChildElement( "file" )->GetText();
		map.intro_file = map_elem->FirstChildElement( "intro" )->Attribute("file");
		if(value > 1)
			map.locked = true;
		else
			map.locked = false;

		if(value < num_map + 1)
		{
			std::cout << "RE_LevelManager: map \"" << map.name << "\" has wrong index( " << value << " < " << num_map + 1<< ") skip it\n";
			continue;
		}
		//std::cout << "RE_LevelManager: checking map " << value << " : " << map.name <<std::endl;

		Maps.push_back(map);

		int num_submap = 0;
		for(tinyxml2::XMLElement* submap_elem = map_elem->FirstChildElement( "submap" ); submap_elem != NULL; submap_elem = submap_elem->NextSiblingElement("submap"))
		{
			RE_Submap submap;

			int svalue = -1;
			submap_elem->QueryIntAttribute( "index", &svalue );

			if(svalue < num_submap + 1)
			{
				std::cout << "RE_LevelManager: submap has wrong index( " << svalue << " < " << num_submap + 1<< ") skip it\n";
				continue;
			}

			submap.file = submap_elem->FirstChildElement( "file" )->GetText();


			Maps[num_map].Submaps.push_back(submap);

			num_submap++;
		}

	//	  std::cout << "RE_LevelManager: found " << num_submap << " submaps for map " << num_map +1<<"\n";
		Maps[num_map].num_submaps = num_submap;
		num_map++;
	}

	num_maps = num_map;

//	  std::cout << "RE_LevelManager: found " << num_maps << " maps\n";

}

std::string RE_LevelManager::get_file(int map, int submap)
{
	if(map < 1 || map > num_maps)
	{
		std::cerr << "RE_levelManager: invalid map number (min:1, max:"<< num_maps<<")\n";
		return "";
	}
	if(submap < 1 || submap > Maps[map-1].num_submaps)
	{
		std::cerr << "RE_levelManager: invalid submap number (min:1, max:"<< Maps[map-1].num_submaps<<")\n";
		return "";
	}
	return Maps[map-1].Submaps[submap-1].file;
}

std::string RE_LevelManager::get_global_file(int map)
{
	if(map < 1 || map > num_maps)
	{
		std::cerr << "RE_levelManager: invalid map number (min:1, max:"<< num_maps<<")\n";
		return "";
	}
	return Maps[map-1].global_file;
}

std::string RE_LevelManager::get_intro_file(int map)
{
	if(map < 1 || map > num_maps)
	{
		std::cerr << "RE_levelManager: invalid map number (min:1, max:"<< num_maps<<")\n";
		return "";
	}
	return Maps[map-1].intro_file;
}

std::string RE_LevelManager::get_map_name(int map)
{
	if(map < 1 || map > num_maps)
	{
		std::cerr << "RE_levelManager: invalid map number (min:1, max:"<< num_maps<<")\n";
		return "";
	}
	return Maps[map-1].name;
}

bool RE_LevelManager::is_map_locked(int map)
{
	if(map < 1 || map > num_maps)
	{
		std::cerr << "RE_levelManager: invalid map number (min:1, max:"<< num_maps<<")\n";
		return true;
	}
	return Maps[map-1].locked;
}

void RE_LevelManager::map_unlock(int map)
{
	if(map < 1 || map > num_maps)
	{
		std::cerr << "RE_levelManager: invalid map number (min:1, max:"<< num_maps<<")\n";
		return;
	}
	//int i=1;
	//for(i=1; i < map; i++)
	//{
	//	if(is_map_locked(i))
	//	{
	//		std::cerr << "RE_levelManager: can't unlock map "<< map <<" since map "<< i <<" is still locked\n";
	//		return;
	//	}
	//}

	Maps[map-1].locked = false;
}
