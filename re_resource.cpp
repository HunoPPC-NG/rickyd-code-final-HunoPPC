/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "re_resource.h"

//focus for input resources (GUI)
static std::map<int,bool> input_focus;

RE_Resource::RE_Resource(RE_GameContext* pgc, RE_Events* pevents, RE_ResourceManager *res)
{
	events = pevents;
	gc = pgc;
	resource = res;
	is_input = false;

	if(events) //input resource if we have a pointer to events class
	{
		insert_input();
	}
}

RE_Resource::~RE_Resource()
{
	bool had_focus = false;
	if(is_input) //is input resource (GUI)
	{
		if(has_focus())
			had_focus = true;

		//remove the input from the list
		input_focus.erase(index);
		if(input_focus.empty())
			events->close_text_input();
	}

	if(had_focus)
	{
		if(input_focus.size() > 0)
		{
			std::map<int,bool>::iterator iter = input_focus.begin();
			int ind = iter->first; //get the first input index
			grab_focus(ind); //set focus to it;
		}
	}
}

int RE_Resource::insert_input()
{
	pair<map<int,bool>::iterator,bool> ret;
	int tries = 10;

	is_input = true;
	index = get_upper_index() + 1; // 0 indexed
	ret = input_focus.insert(pair<int, bool>(index, true)); //add new input with focus set
	//just in case
	while(ret.second==false && tries > 0) // element index already exists
	{
		index++; //increment index
		tries--; //decrement tries
		ret = input_focus.insert(pair<int, bool>(index, true)); //try again
	}

	grab_focus();
}

void RE_Resource::grab_focus(int res_ind)
{
	int ind = index;//use current index
	if(res_ind >= 0)
		ind = res_ind; //use passed value

	std::map<int,bool>::iterator iter;

	for(iter = input_focus.begin(); iter != input_focus.end(); iter++ )
	{
		if(iter->first != ind)
			iter->second = false;
		else
			iter->second = true;
	}
}

int RE_Resource::get_next_index()
{
	if(input_focus.empty())
		return -1;
	//std::map<int,bool>::iterator itup = input_focus.upper_bound (index);
	std::map<int,bool>::iterator iter;

	for(iter = input_focus.begin(); iter != input_focus.end(); iter++ )
	{
		if(iter->first > index)
			return iter->first;
	}

	return input_focus.begin()->first; //the first element
}

int RE_Resource::get_upper_index()
{
	if(input_focus.empty())
		return -1;

	std::map<int,bool>::iterator iter;

	int max = 0;

	for(iter = input_focus.begin(); iter != input_focus.end(); iter++ )
	{

		if(iter->first > max)
			max = iter->first;
	}

	return max;
}

void RE_Resource::check_tab_event()
{
	if(has_focus())
	{
		if(events->keys[RE_Events::KEY_TAB]) //move focus to next input
		{
			//reset
			events->keys[RE_Events::KEY_TAB] = false;

			int next_ind = get_next_index();

			grab_focus(next_ind);
		}
	}
}

bool RE_Resource::has_focus()
{
	return input_focus[index];
}
