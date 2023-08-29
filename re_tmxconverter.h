/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_tmxconverter
#define header_re_tmxconverter

#include "precomp.h"
#include "tinyxml2.h"

class RE_TmxConverter
{
public:
	RE_TmxConverter(std::string tmx_file, std::string m_file = "output_map.xml");
	~RE_TmxConverter();
	
	void convert();
private:
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* map_elem;
	tinyxml2::XMLElement* tileset_elem;
	std::string map_file;
};

#endif