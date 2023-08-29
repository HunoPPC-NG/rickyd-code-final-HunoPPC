/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_scores
#define header_re_scores

#include "precomp.h"

class Score
{
public:
	std::string name;
	int value;
};

class Scores
{
public:
	Scores();
	~Scores();
	void add(Score* sc);
	int size();
	Score* get_score();
	Score* first();
	Score* next();
	void sort_asc();
	void sort_desc();
	bool check_score(int value);
private:
	std::list<Score*> scores_list;
	std::list<Score*>::iterator itr;
};

#endif