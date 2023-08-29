/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "precomp.h"
#include "application.h"
#include "world.h"
#include "re_scores.h"
#include "re_gamecontext.h"
#include "re_optionparser.h"
#include "re_helpers.h"
#include "re_tmxconverter.h"

#include "libHJWSDL2/HunoJoyWrapper.h"

extern SDL_Event event;

void Application::write_conf()
{
	//defaults

	//default score names
	std::vector<std::string> default_names;
	default_names.push_back("LIO");
	default_names.push_back("OLRICK");
	default_names.push_back("ALEX");
	default_names.push_back("BATTEMAN");
	default_names.push_back("MATTHIS");
	default_names.push_back("HUNOPPC");
	default_names.push_back("X1000");
	default_names.push_back("X5000");

	std::string conf_file = HOME_DIR;

	conf_file += "/.rickrc";

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLNode* res_element = doc->InsertEndChild( doc->NewElement( "resources" ) );
	tinyxml2::XMLNode* loc_element = res_element->InsertEndChild( doc->NewElement( "local" ) );
	tinyxml2::XMLElement* win_element = doc->NewElement( "window" );
	win_element->SetAttribute( "width", DEF_WIDTH );
	win_element->SetAttribute( "height", DEF_HEIGHT );
	loc_element->InsertFirstChild( win_element );
	tinyxml2::XMLElement* data_element = doc->NewElement( "data" );
	data_element->SetAttribute( "path", data_dir.c_str() );
	loc_element->InsertFirstChild( data_element );

	tinyxml2::XMLNode* scores_element = loc_element->InsertEndChild(doc->NewElement( "scores" ));
	for(int i=0; i<default_names.size(); i++)
	{
		tinyxml2::XMLElement* score_element = doc->NewElement( "score" );
		score_element->SetAttribute( "name", default_names[i].c_str() );
		score_element->SetAttribute( "value", (int) default_names.size()*1000 - i*1000 );

		scores_element->InsertFirstChild( score_element );
	}

	doc->SaveFile(conf_file.c_str());

	delete(doc);
}

void Application::write_conf(RE_GameContext &gc, World* world)
{
	std::string conf_file = HOME_DIR;

	conf_file += "/.rickrc";

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLNode* res_element = doc->InsertEndChild( doc->NewElement( "resources" ) );
	tinyxml2::XMLNode* loc_element = res_element->InsertEndChild( doc->NewElement( "local" ) );
	tinyxml2::XMLElement* win_element = doc->NewElement( "window" );
	win_element->SetAttribute( "width", gc.get_window_width() );
	win_element->SetAttribute( "height", gc.get_window_height() );
	loc_element->InsertFirstChild( win_element );
	tinyxml2::XMLElement* data_element = doc->NewElement( "data" );
	data_element->SetAttribute( "path", data_dir.c_str() );
	loc_element->InsertFirstChild( data_element );

	tinyxml2::XMLNode* keys_element = loc_element->InsertEndChild(doc->NewElement( "keys" ));

	tinyxml2::XMLElement* key_up_element = doc->NewElement( "up" );
	key_up_element->SetAttribute( "code", world->events->get_key_map(RE_Events::KEY_UP));
	keys_element->InsertFirstChild( key_up_element );

	tinyxml2::XMLElement* key_down_element = doc->NewElement( "down" );
	key_down_element->SetAttribute( "code", world->events->get_key_map(RE_Events::KEY_DOWN));
	keys_element->InsertFirstChild( key_down_element );

	tinyxml2::XMLElement* key_left_element = doc->NewElement( "left" );
	key_left_element->SetAttribute( "code", world->events->get_key_map(RE_Events::KEY_LEFT));
	keys_element->InsertFirstChild( key_left_element );

	tinyxml2::XMLElement* key_right_element = doc->NewElement( "right" );
	key_right_element->SetAttribute( "code", world->events->get_key_map(RE_Events::KEY_RIGHT));
	keys_element->InsertFirstChild( key_right_element );

	tinyxml2::XMLElement* key_action_element = doc->NewElement( "action" );
	key_action_element->SetAttribute( "code", world->events->get_key_map(RE_Events::KEY_ACTION));
	keys_element->InsertFirstChild( key_action_element );

	tinyxml2::XMLElement* key_pause_element = doc->NewElement( "pause" );
	key_pause_element->SetAttribute( "code", world->events->get_key_map(RE_Events::KEY_PAUSE));
	keys_element->InsertFirstChild( key_pause_element );

	tinyxml2::XMLElement* key_quit_element = doc->NewElement( "quit" );
	key_quit_element->SetAttribute( "code", world->events->get_key_map(RE_Events::KEY_QUIT));
	keys_element->InsertFirstChild( key_quit_element );

	loc_element->InsertEndChild( keys_element );

	tinyxml2::XMLNode* scores_element = loc_element->InsertEndChild(doc->NewElement( "scores" ));
	Score* score;
	int sc_ind = 0;
	for(score = world->scores->first(); score != NULL; score = world->scores->next())
	{
		tinyxml2::XMLElement* score_element = doc->NewElement( "score" );
		score_element->SetAttribute( "name", score->name.c_str() );
		score_element->SetAttribute( "value", score->value );

		scores_element->InsertFirstChild( score_element );

		sc_ind++;
		if(sc_ind >= 10)
			break; //only store the 10 higher scores
	}
	loc_element->InsertEndChild( scores_element );

	tinyxml2::XMLNode* levels_element = loc_element->InsertEndChild(doc->NewElement( "levels" ));
	int map = 1;
	for(map = 1; map <= world->Levels->get_num_maps(); map++)
	{
		tinyxml2::XMLElement* level_element = doc->NewElement( "level" );
		level_element->SetAttribute( "index", map );
		if(map == 1)
			level_element->SetAttribute( "locked", false);
		else
			level_element->SetAttribute( "locked", world->Levels->is_map_locked(map));

		levels_element->InsertFirstChild( level_element );
	}
	loc_element->InsertEndChild( levels_element );

	doc->SaveFile(conf_file.c_str());

	delete(doc);
}

RE_ResourceManager* Application::read_conf()
{
	std::string conf_file = HOME_DIR;

	conf_file += "/.rickrc";
	RE_ResourceManager* local_resources = new RE_ResourceManager(conf_file, true);
	if(local_resources->get_error())
	{
		//write default conf and try again
		write_conf();
		delete(local_resources);
		local_resources = new RE_ResourceManager(conf_file, true);
		if(local_resources->get_error())
		{
			std::cerr << "Error while loading local conf:" << conf_file << "\n";
			exit(1);
		}
	}

	return local_resources;
}

int Application::main(const std::vector<std::string> &args)
{
	//data_dir = RICKY_DATA_DIR;

    data_dir = "PROGDIR:data/"/*RICKY_DATA_DIR*/;


    //init HjW here HUnoPPC
    InitHunoJoyWrapper();

	RE_OptionParser opt;

	opt.overview = "RickyD - A Rick Dangerous clone fully configurable by the use of xml";
	opt.syntax = "Usage: rick [OPTIONS]";
	opt.example = "Example: rick --level 1,1\n\n";
	opt.footer = "This program is released under the GPL v2 or latter.\n";

	opt.add(
		"help", //name
		"",     // Default.
		0,      // Required?
		0,      // Number of args expected.
		0,      // Delimiter if expecting multiple args.
		"",     // args help
		"Display usage instructions.", // Help description.
		"-h",     // Flag token.
		"-help",  // Flag token.
		"--help", // Flag token.
		"--usage" // Flag token.
	);

	opt.add(
		"level",
		"1,1",   // Default.
		0,       // Required?
		2,       // Number of args expected.
		',',     // Delimiter if expecting multiple args.
		"map,submap", // args help
		"Start in level given by map and submap.", // Help description.
		"-l",      // Flag token.
		"-level",  // Flag token.
		"--level"  // Flag token.
	);

	opt.add(
		"size",
		"0,0",   // Default.
		0,       // Required?
		2,       // Number of args expected.
		'x',     // Delimiter if expecting multiple args.
		"widthxheight", // args help
		"Set window size", // Help description.
		"-s",      // Flag token.
		"-size",  // Flag token.
		"--size"  // Flag token.
	);

	opt.add(
		"data",
		"PROGDIR:data/",   // Default.
		0,      // Required?
		1,      // Number of args expected.
		' ',     // Delimiter if expecting multiple args.
		"DATA_PATH", // args help
		"change data path", // Help description.
		"-d",     // Flag token.
		"-data",  // Flag token.
		"--data"  // Flag token.
	);

	opt.add(
		"convert",
		"1,1",   // Default.
		0,       // Required?
		2,       // Number of args expected.
		',',     // Delimiter if expecting multiple args.
		"tmx_file,map_file", // args help
		"convert tiled file(tmx) to map file.", // Help description.
		"-c",      // Flag token.
		"-convert",  // Flag token.
		"--convert"  // Flag token.
	);

	opt.add(
		"c4a",
		"",   	// Default.
		0,      // Required?
		0,      // Number of args expected.
		0,      // Delimiter if expecting multiple args.
		"",     // args help
		"activate c4a", // Help description.
		"-c4a",  // Flag token.
		"--c4a",  // Flag token.
		""
	);

	opt.parse(args);

	//load local_resources
	RE_ResourceManager* local_resources = read_conf();

	tinyxml2::XMLElement* local_elem = local_resources->getElement("local");

	int C4A = 0;
	int width =  DEF_WIDTH;
	int height = DEF_HEIGHT;
	tinyxml2::XMLElement* win_elem = local_elem->FirstChildElement("window");
	if(win_elem != NULL)
	{
		win_elem->QueryIntAttribute("width", &width);
		win_elem->QueryIntAttribute("height", &height);
	}

	tinyxml2::XMLElement* data_elem = local_elem->FirstChildElement("data");
	if(data_elem != NULL)
	{
		const char* loc_data_dir = data_elem->Attribute("path");
		if(loc_data_dir != NULL)
			data_dir = loc_data_dir;
	}

	int map = 1;
	int submap = 1;

	if(opt.has_errors())
	{
		std::cerr << opt.display_errors();
		std::cout << opt.display_help();
		return(-1);
	}
	else
	{
		if(opt.has_option("help"))
		{
			std::cout << opt.display_help();
			return(0);
		}

		if(opt.has_option("level"))
		{
			std::vector<std::string> level = opt.get_args("level");

			RE_Helpers *helper = new RE_Helpers();
			map = helper->text_to_int(level[0]);
			submap = helper->text_to_int(level[1]);
			delete(helper);
		}

		if(opt.has_option("size"))
		{
			std::vector<std::string> size = opt.get_args("size");

			RE_Helpers *helper = new RE_Helpers();
			width = helper->text_to_int(size[0]);
			height = helper->text_to_int(size[1]);
			delete(helper);
		}

		if(opt.has_option("data"))
		{
			std::vector<std::string> data = opt.get_args("data");

			data_dir = data[0];
		}

		if(opt.has_option("convert"))
		{
			std::vector<std::string> files = opt.get_args("convert");
			RE_TmxConverter* tmx_converter = new RE_TmxConverter(files[0], files[1]);
			tmx_converter->convert();
			delete(tmx_converter);
			return(0);
		}

		if(opt.has_option("c4a"))
		{
			C4A = 1;
		}
	}

//	  std::cout << "" << "\n";

//	  std::cout << "Data dir:" << data_dir << "\n";

	RE_ResourceManager game_resources("global.xml", false, data_dir);

    RE_GameContext gc("global/window", game_resources, width, height);

	World* world = new World( &gc, &game_resources, map, submap);

	tinyxml2::XMLElement* keys_elem = local_elem->FirstChildElement("keys");
	if(keys_elem)
	{
		tinyxml2::XMLElement* up_key_element = keys_elem->FirstChildElement("up");
		if(up_key_element)
			world->events->set_key_map(RE_Events::KEY_UP, up_key_element->IntAttribute("code"));

		tinyxml2::XMLElement* down_key_element = keys_elem->FirstChildElement("down");
		if(down_key_element)
			world->events->set_key_map(RE_Events::KEY_DOWN, down_key_element->IntAttribute("code"));

		tinyxml2::XMLElement* left_key_element = keys_elem->FirstChildElement("left");
		if(left_key_element)
			world->events->set_key_map(RE_Events::KEY_LEFT, left_key_element->IntAttribute("code"));

		tinyxml2::XMLElement* right_key_element = keys_elem->FirstChildElement("right");
		if(right_key_element)
			world->events->set_key_map(RE_Events::KEY_RIGHT, right_key_element->IntAttribute("code"));

		tinyxml2::XMLElement* action_key_element = keys_elem->FirstChildElement("action");
		if(action_key_element)
			world->events->set_key_map(RE_Events::KEY_ACTION, action_key_element->IntAttribute("code"));

		tinyxml2::XMLElement* pause_key_element = keys_elem->FirstChildElement("pause");
		if(pause_key_element)
			world->events->set_key_map(RE_Events::KEY_PAUSE, pause_key_element->IntAttribute("code"));

		tinyxml2::XMLElement* quit_key_element = keys_elem->FirstChildElement("quit");
		if(quit_key_element)
			world->events->set_key_map(RE_Events::KEY_QUIT, quit_key_element->IntAttribute("code"));
	}

	tinyxml2::XMLElement* scores_elem = local_resources->getElement("local/scores");
	if(scores_elem)
	{
		tinyxml2::XMLElement* score_element = NULL;
		for(score_element = scores_elem->FirstChildElement("score"); score_element != NULL; score_element = score_element->NextSiblingElement("score"))
		{
			Score* sc = new Score();
			sc->name  = score_element->Attribute("name");
			sc->value = score_element->IntAttribute("value");

			world->scores->add(sc);
		}
		world->scores->sort_desc();
	}

	tinyxml2::XMLElement* levels_elem = local_resources->getElement("local/levels");
	if(levels_elem)
	{
		tinyxml2::XMLElement* level_element = NULL;
		for(level_element = levels_elem->FirstChildElement("level"); level_element != NULL; level_element = level_element->NextSiblingElement("level"))
		{
			int map  = level_element->IntAttribute("index");
			bool locked = level_element->IntAttribute("locked");

			if(!locked)
				world->Levels->map_unlock(map);
		}
	}

	delete(local_resources);

	world->c4a = C4A;

	world->run();

	write_conf(gc, world);

    //Remove HjW here HunoPPC
    ShutdownHunoJoyWrapper ();

	delete(world);

	return 0;
}
