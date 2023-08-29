/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "re_point.h"

bool RE_Point::is_inside(RE_Rect other)
{
	return !((y > other.Bottom()) || (y < other.Top()) ||
             (x > other.Right()) || (x < other.Left()));
}

bool RE_Point::is_inside(RE_Rect* other)
{
	return !((y > other->Bottom()) || (y < other->Top()) ||
             (x > other->Right()) || (x < other->Left()));
}