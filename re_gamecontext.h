/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_gamecontext
#define header_re_gamecontext

#define RE_COLOR_BLACK 0,0,0
#define RE_COLOR_WHITE 255,255,255
#define RE_COLOR_BLUE  0,0,255


#include "precomp.h"
#include "re_resourcemanager.h"

class InitError: public std::exception {
    public:
        InitError();
        InitError(const std::string&);
        virtual ~InitError() throw();
        virtual const char* what() const throw();
    private:
        std::string msg;
};

class SDL {
public:
	enum RE_Color
	{
		Color_Black = 0,
		Color_White,
		Color_Red,
		Color_Green,
		Color_Blue
	};
public:
        SDL(Uint32 flags = 0) throw(InitError);
        virtual ~SDL();
		void setWindow(int width, int height) throw(InitError);
		void setWindowIcon(std::string icon_file);
		SDL_Color get_color(RE_Color re_color);
		void window_clear(RE_Color re_color);
		void show(SDL_Texture * frame);
		SDL_Window* get_window(){return window;}
		SDL_Rect viewport;
		SDL_Renderer* renderer;
private:
		SDL_Window* window;
};


class RE_GameContext : public SDL
{
public:
	RE_GameContext(std::string xml_path, RE_ResourceManager &resource, int window_width = 0, int window_height = 0);
	RE_GameContext(int width, int height, int window_width = 0, int window_height = 0);
	~RE_GameContext();
	void openWindow(int width, int height, int window_width = 0, int window_height = 0);
	void setIcon(std::string icon_file);
	void draw(SDL_Texture* img, SDL_Rect* area, int x, int y);
	void clear(RE_Color re_color);
	void flush();
	int get_window_width() {return viewport.w;}
	int get_window_height() {return viewport.h;}
	void set_window_size(int width, int height);
	int get_width() {return renderer_width;}
	int get_height(){return renderer_height;}
private:
	int renderer_width;
	int renderer_height;
	SDL_Texture* render_texture;
};

#endif
