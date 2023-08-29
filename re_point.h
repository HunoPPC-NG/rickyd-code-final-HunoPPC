/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#ifndef header_re_point
#define header_re_point

#include "precomp.h"
#include "re_rect.h"

class RE_Point
{
public:
	bool is_inside(RE_Rect other);
	bool is_inside(RE_Rect* other);

	int x;
	int y;
};

#endif
