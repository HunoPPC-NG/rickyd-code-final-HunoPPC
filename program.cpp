/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include "precomp.h"
#include "application.h"

#ifdef __amigaos4__
const char *version_tag = "$VER: rickyd Version 0.2.0 AOS4 (29.06.2017) port for VillersWEST by HunoPPC Hugues Nouvel";
#endif

int main(int argc, char** argv)
{	
	Application app;
	
	std::vector<std::string> args(argv + 1, argv + argc);

	int ret = app.main(args);
	
	return ret;
}
