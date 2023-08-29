/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "re_scores.h"

Scores::Scores()
{
}

Scores::~Scores()
{
	for(
		std::list<Score*>::iterator iter = scores_list.begin(); 
		iter != scores_list.end(); 
		iter++ )
	{
		delete *iter;
	}
	scores_list.clear();
}

void Scores::add(Score* sc)
{
	scores_list.push_back(sc);
}

int Scores::size()
{
	return scores_list.size();
}

Score* Scores::get_score()
{
	if(itr != scores_list.end() )
		return *itr;
	else
		return NULL;
}

Score* Scores::first()
{
	itr = scores_list.begin();
	return get_score();
}

Score* Scores::next()
{
	itr++;
	return get_score();
}

bool compare_asc(Score* first, Score* second)
{
	bool ret = false;
	
	if(first->value < second->value)
		ret = true;
		
	return ret;
}

bool compare_desc(Score* first, Score* second)
{
	bool ret = false;
	
	if(first->value > second->value)
		ret = true;
		
	return ret;
}

void Scores::sort_asc()
{
	scores_list.sort(compare_asc);
}

void Scores::sort_desc()
{
	scores_list.sort(compare_desc);
}

bool Scores::check_score(int val)
{
	for(
		std::list<Score*>::iterator iter = scores_list.begin(); 
		iter != scores_list.end(); 
		iter++ )
	{
		if(val > ((*iter)->value))
			return true;
	}
	return false;
}