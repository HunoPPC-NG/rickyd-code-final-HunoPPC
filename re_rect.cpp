/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "re_rect.h"

RE_Rect::RE_Rect(int new_left, int new_top, int new_right, int new_bottom)
{
	left = new_left;
	top = new_top;
	right = new_right;
	bottom = new_bottom;

	if(left > right)
	{
		std::cerr <<"RE_Rect: left ("<<left<<") > right ("<<right<<") setting right = left\n";
		right = left;
	}

	if(top > bottom)
	{
		std::cerr <<"RE_Rect: top ("<<top<<") > bottom ("<<bottom<<") setting bottom = top\n";
		bottom = top;
	}
}

RE_Rect::~RE_Rect()
{
}

bool RE_Rect::is_overlapped(RE_Rect other)
{
	return !((top > other.bottom) || (bottom < other.top) ||
             (left > other.right) || (right < other.left));
}

bool RE_Rect::is_overlapped(RE_Rect *other)
{
	return !((top > other->bottom) || (bottom < other->top) ||
             (left > other->right) || (right < other->left));
}
