/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "score_dialog.h"

ScoreDialog::ScoreDialog(RE_GameContext* pgc, RE_ResourceManager* resources, RE_Events* ev, Scores* sc, int score_value)
{
	gc = pgc;
	global_resources = resources;
	events = ev;
	scores = sc;
	score = score_value;

	title_font = new RE_Font(gc, "global/objects/dialog/font", global_resources);
	title_font->resize(24);
	title_font->set_fg_color(0xe8, 0x9f, 0x11, 0x00); //e89f11
	title_font->set_text("SCORES");

	dialog_font = new RE_Font(gc, "global/objects/dialog/font", global_resources);
	dialog_font->resize(14);
	std::ostringstream scoreSStream;
	scoreSStream << "Final Score: " << score;
	dialog_font->set_text(scoreSStream.str());

	button_save = new RE_Button(gc, "global/objects/button", global_resources, events);
	button_save->add_label("Save");

	input_box = new RE_InputBox(gc, "global/objects/input", global_resources, events);
	input_box->set_label("Player Name:");
	input_box->grab_focus();

	commited = false;
	response = UNKNOWN;
}

ScoreDialog::~ScoreDialog()
{
	delete(title_font);
	delete(dialog_font);
	delete(input_box);
	delete(button_save);
}

void ScoreDialog::update(int time_elapsed)
{
	input_box->update(time_elapsed);
	button_save->update(time_elapsed);

	if( button_save->is_commited() ||
		events->keys[RE_Events::KEY_RETURN])
	{
		//add new score to the scores list
		Score* my_score = new Score();
		my_score->value = score;
		my_score->name = input_box->get_text();
		scores->add(my_score);
		scores->sort_desc();

		commited = true;
		response = OK;
	}
}

void ScoreDialog::show(int time_elapsed)
{
	title_font->draw(250, 20);
	dialog_font->draw(80,100);
	input_box->draw(80, 180);
	button_save->draw(260, 260);
}
