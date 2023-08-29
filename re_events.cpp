/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include "re_events.h"

#include "libHJWSDL2/HunoJoyWrapper.h"


int protectB6 = 0;
int protectB1 = 0;
int protectB0 = 0;

RE_Events::RE_Events()
{
	keys[KEY_QUIT] = false;
	keys[KEY_UP] = false;
	keys[KEY_DOWN] = false;
	keys[KEY_LEFT] = false;
	keys[KEY_RIGHT] = false;
	keys[KEY_ACTION] = false;
	keys[KEY_PAUSE] = false;
	keys[KEY_RETURN] = false;

	default_key_map();

	fullscreen = false;
	inputing_text = false;
	commit_text = false;
	mouse_click = false;

	input_text = "";
	input_backspace = false;
	listening_keys = false;

}
RE_Events::~RE_Events()
{

}

void RE_Events::set_key_map(KEY_CODE key, SDL_Keycode sdl_key)
{
	mapkey[key] = sdl_key;
}

SDL_Keycode RE_Events::get_key_map(KEY_CODE key)
{
	return mapkey[key];
}

void RE_Events::default_key_map()
{
	//map all of the game key codes to the defaut sdl key
	set_key_map(KEY_UP, SDLK_UP);
	set_key_map(KEY_DOWN, SDLK_DOWN);
	set_key_map(KEY_RIGHT, SDLK_RIGHT);
	set_key_map(KEY_LEFT, SDLK_LEFT);
	set_key_map(KEY_ACTION, SDLK_LSHIFT);
	set_key_map(KEY_PAUSE, SDLK_p);
	set_key_map(KEY_RETURN, SDLK_RETURN);
	set_key_map(KEY_QUIT, SDLK_ESCAPE);
}

void RE_Events::process()
{
	SDL_Event event;
	int events_processed = 0;

    Uint8* joyHJW;
    joyHJW = GetJoyStateHJW();

	SDL_PumpEvents();

	while(SDL_PollEvent(&event) != 0 /* && events_processed < 20*/)
	{
		events_processed++;
		switch(event.type)
		{ 	/* Process the appropriate event type */
			case SDL_KEYDOWN:  /* Handle a KEYDOWN event */
				on_key_down(&event);
				break;
			case SDL_KEYUP:
				on_key_up(&event);
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				on_mouse_button_down(&event);
				break;
			case SDL_MOUSEBUTTONUP:
				on_mouse_button_up(&event);
				break;
			case SDL_WINDOWEVENT:
				 on_window_event(&event);
				break;
#ifdef __WIN32
			case SDL_TEXTINPUT:
				if(inputing_text)
				{
					/* Add new text onto the end of our text */
					input_text += event.text.text;
					std::cout << "text input:" << event.text.text << "\n";
				}
				break;
			case SDL_TEXTEDITING:
				/*
				Update the composition text.
				Update the cursor position.
				Update the selection length (if any).
				*/
				if(inputing_text)
				{
					input_text = event.edit.text;
					cursor = event.edit.start;
					selection_len = event.edit.length;
					std::cout << "text edit:" << event.edit.text << " cursor:"<<cursor<<" selection:"<<selection_len<<"\n";
				}
				break;
#endif
			default: /* Report an unhandled event */
				//std::cout << "I don't know what this event is!\n";
				break;
		}

	}

    if(event.type == SDL_JOYAXISMOTION) {
		if(event.jaxis.axis == 0 && event.jaxis.which == 0) {
					if(event.jaxis.value > -10000 && event.jaxis.value < 10000){
                        keys[KEY_LEFT] = false;
                        keys[KEY_RIGHT] = false;
					}else if(event.jaxis.value > 10000){
                        keys[KEY_RIGHT] = true;
					}else{
                        keys[KEY_LEFT] = true;
                        }
				}
		if(event.jaxis.axis == 1 && event.jaxis.which == 0) {
   					if(event.jaxis.value > -10000 && event.jaxis.value < 10000){
                        keys[KEY_DOWN] = false;
                        keys[KEY_UP] = false;
					}else if(event.jaxis.value > 10000){
                        keys[KEY_DOWN] = true;
					}else{
                        keys[KEY_UP] = true;
                    }
				}//first joystick NUMBER 1 AXIS MOTION

             }//SDL_JOYAXISMOTION END

    else if ( event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP ){
    // Bouton appuyé ou relâché
    // Nous devons donc utiliser le champ jbutton
    if ( event.jbutton.state == SDL_PRESSED )
   {
    	//joystick player 1
       if(protectB6 == 0 || joyHJW[JOY_HJW_B6]==1){
    //   printf("<ACTION> is pressed.\n");
       keys[KEY_ACTION] = true;
       protectB6 = 1;
       }


    }

    if ( event.jbutton.state == SDL_RELEASED )
    {
    	//JOYSTICK 1 released
    	if(protectB6 == 1 || joyHJW[JOY_HJW_B6]==0){
      //  printf("<ACTION> is released.\n");
        keys[KEY_ACTION] = false;
        protectB6 = 0;
       }


    }


}


/*
	Uint8 *state = SDL_GetKeyboardState(NULL);
	if ( state[SDL_SCANCODE_RETURN] ) {
		printf("<RETURN> is pressed.\n");
		keys[KEY_RETURN] = true;
	}
	else keys[KEY_RETURN] = false;

	if ( state[SDL_SCANCODE_DOWN] ) {
		printf("<DOWN> is pressed.\n");
		keys[KEY_DOWN] = true;
	}
	else keys[KEY_DOWN] = false;

	if ( state[SDL_SCANCODE_UP] ) {
		printf("<UP> is pressed.\n");
		keys[KEY_UP] = true;
	}
	else keys[KEY_UP] = false;


	if ( state[SDL_SCANCODE_RSHIFT] ||
	     state[SDL_SCANCODE_LSHIFT]) {
		printf("<ACTION> is pressed.\n");
		keys[KEY_ACTION] = true;
	}
	else keys[KEY_ACTION] = false;

	if ( state[SDL_SCANCODE_LEFT] ) {
		printf("<LEFT> is pressed.\n");
		keys[KEY_LEFT] = true;
	}
	else keys[KEY_LEFT] = false;


	if ( state[SDL_SCANCODE_RIGHT] ) {
		printf("<RIGHT> is pressed.\n");
		keys[KEY_RIGHT] = true;
	}
	else keys[KEY_RIGHT] = false;

	if ( state[SDL_SCANCODE_P] ) {
		printf("<P> is pressed.\n");
		keys[KEY_PAUSE] = true;
	}
	else keys[KEY_PAUSE] = false;

	if ( state[SDL_SCANCODE_ESCAPE] ) {
		printf("<ESCAPE> is pressed.\n");
		keys[KEY_QUIT] = true;
	}
	else keys[KEY_QUIT] = false;
*/

}

void RE_Events::open_text_input()
{
	SDL_StartTextInput();
	//SDL_Rect InputBox;
	//InputBox.x = x;
	//InputBox.y = y;
	//InputBox.w = w;
	//InputBox.h = h;
	//SDL_SetTextInputRect(&InputBox);
	inputing_text = true;
	commit_text = false;
}

void RE_Events::close_text_input()
{
	SDL_StopTextInput();
	input_text.clear();
	inputing_text = false;
	commit_text = false;
}

void RE_Events::on_window_event(SDL_Event* event)
{
	switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            //fprintf(stderr, "Window %d shown", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            //fprintf(stderr, "Window %d hidden", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            //fprintf(stderr, "Window %d exposed", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MOVED:
            //fprintf(stderr, "Window %d moved to %d,%d",
            //        event->window.windowID, event->window.data1,
            //        event->window.data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            //fprintf(stderr, "Window %d resized to %dx%d",
            //        event->window.windowID, event->window.data1,
            //        event->window.data2);
			//window_width  = event->window.data1;
			//window_height = event->window.data2;
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            //fprintf(stderr, "Window %d minimized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            //fprintf(stderr, "Window %d maximized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_RESTORED:
            //fprintf(stderr, "Window %d restored", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_ENTER:
            //fprintf(stderr, "Mouse entered window %d",
            //        event->window.windowID);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            //fprintf(stderr, "Mouse left window %d", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            //fprintf(stderr, "Window %d gained keyboard focus",
            //        event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            //fprintf(stderr, "Window %d lost keyboard focus",
            //        event->window.windowID);
            break;
        case SDL_WINDOWEVENT_CLOSE:
            //fprintf(stderr, "Window %d closed", event->window.windowID);
			keys[KEY_QUIT] = true;
            break;
        default:
            //fprintf(stderr, "Window %d got unknown event %d",
            //        event->window.windowID, event->window.event);
            break;
        }

}

SDL_Keycode RE_Events::listen_for_key()
{
	SDL_Keycode ret = 0;

	if(!listening_keys)
		listening_keys = true;
	else
	{
		ret = key_pressed;
	}

	key_pressed = 0;
	return ret;
}

void RE_Events::stop_listen_for_key()
{
	listening_keys = false;
	key_pressed = 0;
}

void RE_Events::on_key_down(SDL_Event* event)
{
	if(listening_keys)
	{
		key_pressed = event->key.keysym.sym;
		if(key_pressed == SDLK_RETURN)
		{
			key_pressed = 0; //disable the return key
			keys[KEY_RETURN] = true; //flag it (used by input objects)
		}
		if(key_pressed == SDLK_TAB)
		{
			key_pressed = 0; //disable the tab key
			keys[KEY_TAB] = true; //flag it (used by input objects)
		}
		return; //don't flag the keys
	}
	else
		key_pressed = 0;

	//std::cout << "key down:" << event->key.keysym.sym <<"\n";
	/*can't use switch here*/
	if (event->key.keysym.sym == mapkey[KEY_UP])
		keys[KEY_UP] = true;
	else if (event->key.keysym.sym == mapkey[KEY_DOWN])
		keys[KEY_DOWN] = true;
	else if (event->key.keysym.sym == mapkey[KEY_LEFT])
		keys[KEY_LEFT] = true;
	else if (event->key.keysym.sym == mapkey[KEY_RIGHT])
		keys[KEY_RIGHT] = true;
	else if (event->key.keysym.sym == mapkey[KEY_ACTION])
		keys[KEY_ACTION] = true;
	else if (event->key.keysym.sym == mapkey[KEY_PAUSE])
		keys[KEY_PAUSE] = true;
	else if(event->key.keysym.sym == mapkey[KEY_QUIT])
		keys[KEY_QUIT] = true;

#ifndef __WIN32
	if (event->key.keysym.sym == SDLK_SPACE)
	{
		 if(inputing_text)
			input_text += " ";
	}
#endif
	else if (event->key.keysym.sym == SDLK_F1)
	{
		fullscreen = !fullscreen;
		if(fullscreen)
			SDL_SetWindowFullscreen( SDL_GetWindowFromID(event->key.windowID), SDL_TRUE);
		else
			SDL_SetWindowFullscreen( SDL_GetWindowFromID(event->key.windowID), SDL_FALSE);
	}
	else if (event->key.keysym.sym == SDLK_RETURN)
	{
		if(inputing_text)
			commit_text = true;

		keys[KEY_RETURN] = true;
	}
	else if (event->key.keysym.sym == SDLK_TAB)
	{
		keys[KEY_TAB] = true;
	}
	else if (event->key.keysym.sym == SDLK_BACKSPACE)
	{
		if(inputing_text)
		{
			if(input_text.size() > 0)
				input_text.erase(input_text.size() - 1);
			else
				input_backspace = true;
		}
	}

#ifndef __WIN32
    if(inputing_text)
    {
        std::ostringstream my_text;
        my_text << SDL_GetKeyName(event->key.keysym.sym);
        if(my_text.str().size() <= 2)
        {
		    bool all_caps = false;
            Uint32 shift_mod = event->key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT);
            Uint32 caps_mod =  event->key.keysym.mod & KMOD_CAPS;

		    if((shift_mod && !caps_mod) || (!shift_mod && caps_mod))
                all_caps = true;

		    if(all_caps && islower(my_text.str()[0]))
			    input_text += toupper(my_text.str()[0]);
		    else if(!all_caps && isupper(my_text.str()[0]))
			    input_text += tolower(my_text.str()[0]);
		    else
                input_text += my_text.str();
        }
        //std::cout << "key pressed:"<<  my_text.str() <<"("<< event->key.keysym.sym <<") size:"<< my_text.str().size() <<"\n";
    }
#endif
}

void RE_Events::on_key_up(SDL_Event* event)
{
	/*can't use switch here*/
	if (event->key.keysym.sym == mapkey[KEY_UP])
		keys[KEY_UP] = false;
	else if (event->key.keysym.sym == mapkey[KEY_DOWN])
		keys[KEY_DOWN] = false;
	else if (event->key.keysym.sym == mapkey[KEY_LEFT])
		keys[KEY_LEFT] = false;
	else if (event->key.keysym.sym == mapkey[KEY_RIGHT])
		keys[KEY_RIGHT] = false;
	else if (event->key.keysym.sym == mapkey[KEY_ACTION])
		keys[KEY_ACTION] = false;
	else if (event->key.keysym.sym == mapkey[KEY_PAUSE])
		keys[KEY_PAUSE] = false;
	else if(event->key.keysym.sym == mapkey[KEY_QUIT])
		keys[KEY_QUIT] = false;

	if (event->key.keysym.sym == SDLK_RETURN)
		keys[KEY_RETURN] = false;
	else if (event->key.keysym.sym == SDLK_TAB)
		keys[KEY_TAB] = false;
}

bool RE_Events::check_mouse_click(RE_Point* mouse_point)
{
	bool ret = mouse_click;

	if(mouse_click)
	{
		mouse_point->x = mouse_pos.x;
		mouse_point->y = mouse_pos.y;
		//cout << "RE_Events: mouse click (" << mouse_pos.x << ","<< mouse_pos.y << ")\n";
	}

	return ret;
}

void RE_Events::on_mouse_button_down(SDL_Event* event)
{
	switch(event->button.button)
	{
		case SDL_BUTTON_LEFT:
			if(event->button.state == SDL_PRESSED)
			{
				mouse_click = true;
				mouse_pos.x = event->button.x;
				mouse_pos.y = event->button.y;
			}
			break;
		default:
			break;

	}
}

void RE_Events::on_mouse_button_up(SDL_Event* event)
{
	mouse_click = false;
}

std::string RE_Events::get_input_text()
{
	std::string temp_str = input_text;
	input_text.clear();

	return temp_str;
}

bool RE_Events::has_backspace()
{
	bool ret = input_backspace;
	input_backspace = false; //reset

	return ret;
}
