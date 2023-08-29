/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_application
#define header_application

#include "precomp.h"
#include "world.h"
#include "re_resourcemanager.h"

#if _MSC_VER > 1000
#pragma once
#endif

#define DEF_WIDTH  640
#define DEF_HEIGHT 480

class Application
{
//!Operations:
public:
	void write_conf(); //defaults
	void write_conf(RE_GameContext &gc, World* world);
	RE_ResourceManager* read_conf();
	//: Main function for the application.
	virtual int main(const std::vector<std::string> &args);
	
//!Implementation:
private:
	std::string data_dir;
};

#endif
