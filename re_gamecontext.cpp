/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "re_gamecontext.h"
#include "tinyxml2.h"

InitError::InitError():
  exception(), msg(SDL_GetError()) {}

  InitError::InitError(const std::string& m):
  exception(), msg(m) {}

  InitError::~InitError() throw() {}

const char* InitError::what() const throw()
{
    return msg.c_str();
}

SDL::SDL(Uint32 flags) throw(InitError)
{
//	  std::cout << "starting sdl...";
    if (SDL_Init(flags) != 0)
	{
		std::cout << "ERR\n";
        throw InitError();
	}

	int initted = Mix_Init(MIX_INIT_OGG);
	if((initted & MIX_INIT_OGG) != MIX_INIT_OGG)
	{
		std::cerr << "SDL: Mix_Init Failed to init required ogg support! "<< Mix_GetError() <<"\n";
	}

	//open audio
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		std::cerr << "SDL: Unable to initialize audio Mixer: "<< Mix_GetError() << "\n";
		exit(1);
	}

	//open fonts
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL: Unable to initialize SDL_ttf:"<< TTF_GetError()<<"\n";
		exit(1);
	}

//	  std::cout << "OK\n";

	//SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "1");
	//SDL_SetHint("SDL_HINT_RENDER_VSYNC", "1");
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "compositing");
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  //hunoppc

	window = NULL;
}

SDL::~SDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
    SDL_Quit();
}

void SDL::setWindow(int width, int height) throw(InitError)
{
//	  std::cout << "creating window(" << width<<"x"<<height<<")...";
	if(window == NULL)
	{
    	window = SDL_CreateWindow("RickyD AmigaOS4 by HunoPPC 2017",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                width, height,
            //    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |SDL_WINDOW_RESIZABLE);
    }
	else //we already have a window just resize it
	{
		SDL_SetWindowSize(window, width, height);
		return;
	}

	if (window == NULL)
        throw InitError();
//	  std::cout << "OK\n";

	viewport.x = 0;
	viewport.y = 0;
	viewport.h = height;
	viewport.w = width;
	renderer = SDL_CreateRenderer(window, -1,
		//SDL_RENDERER_TARGETTEXTURE |
		SDL_RENDERER_PRESENTVSYNC  |
		SDL_RENDERER_ACCELERATED);

	if(renderer == NULL)
	{
		std::cerr << "SDL: Couldn't get a renderer:" << SDL_GetError() << "\n";
		throw InitError();
	}

//	  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void SDL::setWindowIcon(std::string icon_file)
{
	 //create a surface with the icon image
    SDL_Surface *icon = IMG_Load(icon_file.c_str());

    // The icon is attached to the window pointer
	SDL_SetWindowIcon(window, icon);

	// ...and the surface containing the icon pixel data is no longer required.
	SDL_FreeSurface(icon);

}

SDL_Color SDL::get_color(RE_Color re_color)
{
	SDL_Color color;

	switch(re_color)
	{
		case Color_Black:
			color.r =0;
			color.g =0;
			color.b =0;
			break;
		case Color_White:
			color.r =255;
			color.g =255;
			color.b =255;
			break;
		case Color_Red:
			color.r =255;
			color.g =0;
			color.b =0;
			break;
		case Color_Green:
			color.r =0;
			color.g =255;
			color.b =0;
			break;
		case Color_Blue:
			color.r =0;
			color.g =0;
			color.b =255;
			break;
	}

	return color;
}


void SDL::window_clear(RE_Color re_color)
{
	SDL_Color color = get_color(re_color);

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderClear(renderer);
}

void SDL::show(SDL_Texture * frame)
{
	viewport.x = 0;
	viewport.y = 0;

	SDL_GetWindowSize(window, &viewport.w, &viewport.h);
	//std::cout << "window:" << viewport.w <<"x" << viewport.h << "\n";

	SDL_SetRenderTarget(renderer, NULL);

	if(frame != NULL)
		SDL_RenderCopy(renderer, frame, NULL, NULL);

	SDL_RenderPresent(renderer);
}

RE_GameContext::RE_GameContext(std::string xml_path, RE_ResourceManager &resource, int window_width, int window_height) :
	SDL(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)
{
	tinyxml2::XMLElement* window_elem = resource.getElement(xml_path);

	int width  = window_elem->IntAttribute("width");
	int height = window_elem->IntAttribute("height");
	std::string icon_file = window_elem->Attribute("icon_file");

	openWindow(width, height, window_width, window_height);
	if(icon_file.size() > 2)
	{
		icon_file = resource.get_data_dir() + icon_file;
		setIcon(icon_file);
	}
}

RE_GameContext::RE_GameContext(int width, int height, int window_width, int window_height)
{
	openWindow(width, height, window_width, window_height);
}

RE_GameContext::~RE_GameContext()
{
	SDL_DestroyTexture(render_texture);
}

void RE_GameContext::set_window_size(int width, int height)
{
	setWindow(width, height);
}

void RE_GameContext::openWindow(int width, int height, int window_width, int window_height)
{
	if(window_width > 320 && window_height > 240)
		setWindow(window_width, window_height);
	else
		setWindow(width, height);

	SDL_RenderSetLogicalSize(renderer, width, height);
	SDL_RenderGetLogicalSize(renderer, &renderer_width, &renderer_height);

	render_texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
		width,
		height);

}

void RE_GameContext::setIcon(std::string icon_file)
{
	setWindowIcon(icon_file);
}

void RE_GameContext::draw(SDL_Texture* img, SDL_Rect* area, int x, int y)
{
	SDL_Rect blit_rect;
	blit_rect.x = x;
	blit_rect.y= y;
	blit_rect.w = area->w;
	blit_rect.h = area->h;

	SDL_SetRenderTarget(renderer, render_texture);
	SDL_SetTextureColorMod( img, 255, 255, 255 );
	SDL_RenderCopy(renderer, img, area, &blit_rect);

	//cout << "RE_graphicContext: orig.x("<< area->x <<") orig.y("<< area->y <<") area.w("<< area->w <<") area.h(" << area->h <<")\n";
	//cout << "                   dest.x("<< blit_rect.x <<") dest.y("<< blit_rect.y<<") dest.w("<<blit_rect.w<<") dest.h("<<blit_rect.h<<")\n";

}

void RE_GameContext::clear(RE_Color re_color)
{
	SDL_Color color = get_color(re_color);

	//clear default target
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderClear(renderer);

	//clear render texture
	SDL_SetRenderTarget(renderer, render_texture);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderClear(renderer);
}

void RE_GameContext::flush()
{
	show(render_texture);
}

