/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "re_helpers.h"

std::vector<std::string> RE_Helpers::split(std::string s, char delim)
{
	std::string temp;
	std::vector<std::string> elems(0);
	if (s.size() == 0 || delim == 0)
		return elems;

	for(int i=0; i < s.length(); i++)
	{
		char c = s[i];
		if(c == delim)
		{
			elems.push_back(temp);
			temp.clear();
		}
		else
			temp.push_back(c);
	}
	if (temp.size() > 0)
		elems.push_back(temp);
	return elems;
}

int RE_Helpers::text_to_int(std::string text_int)
{
	int value = atoi(text_int.c_str());

	return value;
}

// std::vector<std::string> RE_Helpers::split(std::string s, std::string delims)
// {
	// std::stringstream temp (std::stringstream::in | std::stringstream::out);
	// std::vector<std::string> elems(0);
	// bool found;
	// if(s.size() == 0 || delims.size() == 0)
		// return elems;
	// for(int i=0; i < s.length(); i++)
	// {
		// char c = s[i];
		// found = false;

		// for(int j =0; j < delims.length(); j++)
		// {
			// char d = delims[j];
			// if (c == d)
			// {
				// elems.push_back(temp.str());
				// temp = std::stringstream(std::stringstream::in | std::stringstream::out);
				// found = true;
				// break;
			// }
		// }
		// if(!found)
			// temp << c;
	// }
	// if(temp.str().size() > 0)
		// elems.push_back(temp.str());
	// return elems;
// }