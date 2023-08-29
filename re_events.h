/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#ifndef header_re_events
#define header_re_events

#include "precomp.h"
#include "re_point.h"

class RE_Events
{
public:
	enum KEY_CODE
	{
		KEY_UP = 0,
		KEY_DOWN,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_ACTION,
		KEY_PAUSE,
		KEY_QUIT,
		KEY_RETURN,
		KEY_TAB,
		NUMBER_OF_KEYS,
	};
public:
	RE_Events();
	~RE_Events();
	void process();
	bool fullscreen;
	bool inputing_text;
	bool commit_text;

public:
	bool keys[NUMBER_OF_KEYS]; //state of keys: down(true) up(false)
	int cursor;
	int selection_len;
	void set_key_map(KEY_CODE key, SDL_Keycode sdl_key);
	SDL_Keycode get_key_map(KEY_CODE key);
	void default_key_map();
	//returns the last key pressed: disables key flags
	//used for setting the game keys
	SDL_Keycode listen_for_key();
	//stop listening for last key pressed: enables key flags
	//must be called after setting the game keys or keyboard will stop
	//reponding
	void stop_listen_for_key();

	void open_text_input();
	void close_text_input();
	bool check_mouse_click(RE_Point* mouse_point);
	std::string get_input_text();
	bool has_backspace();

private:
	SDL_Keycode mapkey[NUMBER_OF_KEYS]; //key map to sdl key code
	bool listening_keys;
	SDL_Keycode key_pressed;
	void on_window_event(SDL_Event* event);
	void on_key_down(SDL_Event* event);
	void on_key_up(SDL_Event* event);
	void on_mouse_button_down(SDL_Event* event);
	void on_mouse_button_up(SDL_Event* event);
	bool mouse_click;
	bool input_backspace;
	RE_Point mouse_pos;
	std::string input_text;
};

#endif
