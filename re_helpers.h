/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#pragma once

#include "precomp.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
class RE_Helpers
{
public:
	std::vector<std::string> split(string s, char delim);
    
	int text_to_int(std::string text_int);
	
        //Splits string s with a list of delimiters in delims (it's just a list, like if we wanted to
        //split at the following letters, a, b, c we would make delims="abc".
	//std::vector<std::string> split(string s, string delims);
};
