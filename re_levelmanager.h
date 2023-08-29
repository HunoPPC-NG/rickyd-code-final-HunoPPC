/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_levelmanager
#define header_re_levelmanager

#include "precomp.h"
#include "tinyxml2.h"

class RE_Submap
{
public:
	std::string file;
};

class RE_Map
{
public:
	std::vector<RE_Submap> Submaps;
	int num_submaps;
	std::string global_file;
	std::string intro_file;
	std::string name;
	bool locked;
};

class RE_LevelManager
{
public:
	RE_LevelManager(std::string file = "", std::string data_dir = "PROGDIR:data/");
	~RE_LevelManager();
	int get_num_maps(){return num_maps;}
	std::string get_file(int map, int submap);
	std::string get_global_file(int map);
	std::string get_intro_file(int map);
	std::string get_map_name(int map);
	bool is_map_locked(int map);
	void map_unlock(int map);

private:
	std::vector<RE_Map> Maps;
	int num_maps;
	void load(std::string file);
};

#endif
