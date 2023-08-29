/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include <fstream>
#include "re_resourcemanager.h"

RE_ResourceManager::RE_ResourceManager(std::string xml_file, bool full_path, std::string new_data_dir)
{
	error = 0;
	data_dir = new_data_dir;

	if(!full_path)
		xml_file = data_dir+xml_file;

	int err = doc.LoadFile(xml_file.c_str());
	if( err != tinyxml2::XML_NO_ERROR)
	{
		std::cerr << "RE_ResourceManager: error ("<<err<<") while loading \""
			<< xml_file <<":";
		switch(err)
		{
			case tinyxml2::XML_NO_ATTRIBUTE:
				std::cerr << " no attribute ("<<doc.GetErrorStr1()<<")";
				break;
			case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
				std::cerr << " wrong attribute ("<<doc.GetErrorStr1()<<")";
				break;
			case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
				std::cerr << " couldn't find file\n";
				break;
			case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
				std::cerr << " couldn't open file\n";
				break;
			case tinyxml2::XML_ERROR_FILE_READ_ERROR:
				std::cerr << " couldn't read file\n";
				break;
			case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
			case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
				std::cerr << " element mismatch ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_PARSING_ELEMENT:
				std::cerr << " failed parsing element ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
				std::cerr << " failed parsing attribute ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
				std::cerr << " failed identifying tag ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_PARSING_TEXT:
				std::cerr << " failed parsing text ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_PARSING_CDATA:
				std::cerr << " failed parsing cdata ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_PARSING_COMMENT:
				std::cerr << " failed parsing comment ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_PARSING_DECLARATION:
				std::cerr << " failed parsing declaration ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
				std::cerr << " failed parsing unknown ("<< doc.GetErrorStr1() <<")";
				break;
			case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
				std::cerr << " empty document";
				break;
			case tinyxml2::XML_ERROR_PARSING:
				std::cerr << " failed parsing ("<< doc.GetErrorStr1() <<")";
				break;
		}
		std::cerr <<"\n";

		if(!full_path)
			exit(1);
		else
		{
			error = -1;
			return;
		}
	}

	root_elem = doc.FirstChildElement( "resources" );
}

RE_ResourceManager::~RE_ResourceManager()
{

}

tinyxml2::XMLElement* RE_ResourceManager::getElement(std::string elem_path, bool warn)
{
	//split path
	RE_Helpers helper;

	std::vector<std::string> elems = helper.split(elem_path, '/');

	tinyxml2::XMLElement* cur_elem = root_elem;
	for(int i=0; i < elems.size(); i++)
	{
		cur_elem = cur_elem->FirstChildElement( elems[i].c_str() );
		if(cur_elem == NULL)
		{
			if(warn)
				std::cerr << "RE_ResourceManager: no "<< elems[i] << " element ("<< elem_path <<") found in resource file\n";
			return NULL;
		}
	}

	return cur_elem;
}

bool RE_ResourceManager::file_exists(std::string xml_path)
{
	tinyxml2::XMLElement* elem = this->getElement(xml_path, false);
	if(!elem)
		return false;

	std::string file = elem->Attribute( "file" );
	file = this->get_data_dir() + file;

	std::ifstream ifs;
	ifs.open (file.c_str(), std::ifstream::in);
	bool err = ifs.fail(); //check for failure

	ifs.close();

	return !err;
}
