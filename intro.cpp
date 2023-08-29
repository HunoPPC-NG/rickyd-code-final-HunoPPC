/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "intro.h"

Intro::Intro(RE_GameContext* pgc, RE_LevelManager* levels, int map)
{
	gc = pgc;
	intro_resources = new RE_ResourceManager(levels->get_intro_file(map));
	intro_clip = new RE_Sprite(gc, "map/intro/movie/clip/sprite", intro_resources);
	intro_strip = new RE_Sprite(gc, "map/intro/movie/strip/sprite", intro_resources);
	intro_font = new RE_Font(gc, "map/intro/font", intro_resources);
	
	sfx_intro = new RE_SoundBuffer("map/intro/music/sample", intro_resources);
	sfx_intro->set_volume(1.0);
	sound = new RE_SoundSession();
	
	intro_frame_ind = 0;
	intro_frame2_ind = 0;
	
	intro_turn = 0;
	intro_strip_turn = 0;
}

Intro::~Intro()
{
	delete(sfx_intro);
	delete(sound);
	delete(intro_font);
	delete(intro_clip);
	delete(intro_strip);
	delete(intro_resources);
}

void Intro::show(int time)
{
	intro_turn += time;
	intro_strip_turn += time;
	
	
	if(!sound->is_playing())
		sound->play(sfx_intro);
		
	tinyxml2::XMLElement* intro_element = intro_resources->getElement("map/intro");
	
	std::string title = intro_element->FirstChildElement("title")->GetText();
		
	intro_font->draw_text(10, 10, title);

	if(intro_turn >= 500)
	{
		intro_turn = 0;
		intro_frame_ind++;
	}
	
	if(intro_strip_turn >= 150)
	{
		intro_strip_turn = 0;
		intro_frame2_ind++;
	}
		
	if(intro_frame_ind > intro_clip->get_frame_count() -1)
		intro_frame_ind = 0;
	
	intro_clip->set_frame(intro_frame_ind);
	
	if(intro_frame2_ind > intro_strip->get_frame_count() -1)
		intro_frame2_ind = 0;
	intro_strip->set_frame(intro_frame2_ind);
	//center intro_clip
	int pic_x = floor(gc->get_width()/2) - floor(intro_clip->get_width()/2);	
	intro_strip->draw(pic_x - (intro_strip->get_width() + 5), 30);
	intro_clip->draw(pic_x, 80);
	intro_strip->draw(pic_x + intro_clip->get_width() + 5, 30);
	
	int ln_num = 0;
	for(tinyxml2::XMLElement* text_elem = intro_element->FirstChildElement( "text" ); 
			text_elem != NULL; 
			text_elem = text_elem->NextSiblingElement("text"))
	{
		std::string line = text_elem->GetText();
		intro_font->draw_text(10, 350 + ln_num * intro_font->get_font_size(), line);
		ln_num++;
	}
}
