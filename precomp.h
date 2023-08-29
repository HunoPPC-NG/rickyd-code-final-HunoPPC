/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <exception>
#include "re_helpers.h"
#include "../config.h"

#ifndef RICKY_DATA_DIR
	#ifdef __WIN32 //use relative path
		#define RICKY_DATA_DIR "../share/ricky/"
	#else
		#define RICKY_DATA_DIR DATA_DIR"/ricky/"
	#endif
#endif

#ifndef HOME_DIR
	#ifdef __WIN32 //use relative path
		#define HOME_DIR "."
	#else
		#define HOME_DIR "/PROGDIR"
		#define __stdcall
	#endif
#endif
