/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#ifndef header_re_rect
#define header_re_rect

#include "precomp.h"

class RE_Rect
{
public:
	RE_Rect(int new_left, int new_top, int new_right, int new_bottom);
	~RE_Rect();
	bool is_overlapped(RE_Rect other);
	bool is_overlapped(RE_Rect *other);

	int Left(){return left;}
	void Left(int new_left){left = new_left;}
	int Top(){return top;}
	void Top(int new_top){top = new_top;}
	int Right(){return right;}
	void Right(int new_right){right = new_right;}
	int Bottom(){return bottom;}
	void Bottom(int new_bottom){bottom = new_bottom;}
private:
	int left;
	int top;
	int right;
	int bottom;
};

#endif
