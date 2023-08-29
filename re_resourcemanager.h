/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_resourcemanager
#define header_re_resourcemanager

#include "precomp.h"
#include "tinyxml2.h"

class RE_ResourceManager
{
public:
	RE_ResourceManager(std::string xml_file, bool full_path = false, std::string new_data_dir = "PROGDIR:data/");
	~RE_ResourceManager();
	tinyxml2::XMLElement* getElement(std::string elem_path, bool warn = true);
	bool file_exists(std::string xml_path);
	int get_error(){return error;}
	std::string get_data_dir(){return data_dir;}

private:
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* root_elem;
	std::string data_dir;
	int error;
};

#endif
