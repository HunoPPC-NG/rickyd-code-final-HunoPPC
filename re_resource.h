/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_resource
#define header_re_resource

#include "precomp.h"
#include "re_events.h"
#include "re_gamecontext.h"

class RE_Resource
{
public:
	RE_Resource(RE_GameContext* pgc=NULL, RE_Events* pevents=NULL, RE_ResourceManager *res=NULL);
	virtual ~RE_Resource();

	//for input resources (GUI)
	bool is_commited(){return commited;}
	void set_commited(bool flag){commited = flag;}
	bool has_focus();
	void grab_focus(int res_ind = -1); //by default use current index
	int insert_input();
	int get_input_index(){return index;}
	void check_tab_event();
	int get_next_index();
	int get_upper_index();

	RE_Events* events;
	RE_GameContext* gc;
	RE_ResourceManager *resource;

private:
	//for input resources (GUI)
	int index; //index of input resource (0 indexed)
	bool is_input; //flag true for input resources
	bool commited;
};
#endif
