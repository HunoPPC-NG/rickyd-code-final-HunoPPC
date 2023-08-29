/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "welcome.h"

Welcome::Welcome(RE_GameContext* pgc, RE_ResourceManager* game_resources, Scores* tb_scores)
{
	welcome_time = WELCOME_TIME;
	score_time = SCORE_TIME;
	gc = pgc;
	scores = tb_scores;

	welcome_screen = new RE_Sprite(gc, "global/welcome/sprite", game_resources);

	welcome_font = new RE_Font(gc, "global/welcome/font", game_resources);
	score_font = new RE_Font(gc, "global/score/font", game_resources);

	sfx_welcome = new RE_SoundBuffer("global/welcome/music/sample", game_resources);
	sfx_welcome->set_volume(1.0);
	sound = new RE_SoundSession();

	turn = 0;
}

Welcome::~Welcome()
{
	delete(sfx_welcome);
	delete(sound);
	delete(welcome_font);
	delete(score_font);
	delete(welcome_screen);
}

void Welcome::show(int time_elapsed)
{
	turn = time_elapsed;

	if(!sound->is_playing())
		sound->play(sfx_welcome);


	if(welcome_time > 0)
	{
		welcome_screen->set_frame(0);
		welcome_screen->draw(0,0);

		welcome_time -= turn;
	}
	else if(score_time > 0)
	{
		//show score
		welcome_font->set_fg_color(0xe8, 0x9f, 0x11, 0x00); //e89f11
		welcome_font->draw_text(250, 20, "SCORES");

		Score* score = scores->first() ;
		int line = 0;
		for(score = scores->first(); score != NULL; score = scores->next())
		{
			std::ostringstream scoreSStream;
			scoreSStream << score->value;

			score_font->draw_text(80, 100 + line * score_font->get_font_size(), score->name);
			score_font->draw_text(500, 100 + line * score_font->get_font_size(), scoreSStream.str());

			line += 2;
			if(line >= 16)
				break; //only show the 8 higher scores
		}

		score_time -= turn;
	}
	else
	{
		welcome_time = WELCOME_TIME;
		score_time = SCORE_TIME;
	}
	//conf file	- best score table
//	tinyxml2::XMLElement* score_element = local_resources->getElement("local/score");
//
//	std::string score_title = score_element->FirstChildElement("title")->GetText();
//
//	welcome_font->draw_text(250, 20, score_title);


//	int ln_num = 0;
//	for(tinyxml2::XMLElement* entry_elem = score_element->FirstChildElement( "entry" );
//			entry_elem != NULL;
//			entry_elem = entry_elem->NextSiblingElement("entry"))
//	{
//		std::string name = entry_elem->Attribute("name");
//		std::string score = entry_elem->Attribute("score");
//		welcome_font->draw_text(50, 80 + ln_num * welcome_font->get_font_size(), name);
//		welcome_font->draw_text(300, 80 + ln_num * welcome_font->get_font_size(), score);
//		ln_num++;
//	}
}
