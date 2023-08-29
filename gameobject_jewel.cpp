/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_jewel.h"


/////////////////////////////////////////////////////////////////////////////
// GameObject_Jewel construction:

GameObject_Jewel::GameObject_Jewel(World *world, tinyxml2::XMLElement* object_element) :
	GameObject_Moving(world)
{
	set_type(bonus_points);
	
	spr_object[active] = new RE_Sprite(world->gc, "global/objects/jewel/active/waiting/sprite", world->global_resources);

	sfx_jewel = new RE_SoundBuffer("global/objects/jewel/sounds/grab/sample", world->global_resources);
	sfx_jewel->set_volume(1.0f);
	
	sound = new RE_SoundSession();

	bonus_value = JEWEL_BONUS;
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	object_element->FirstChildElement("position")->QueryIntAttribute("points", &bonus_value);
	
	std::ostringstream scoreSStream;
	scoreSStream << bonus_value;
	std::string score_text = scoreSStream.str();
	
	score_font = new RE_Font(world->gc, "global/objects/jewel/font", world->global_resources);
	score_font->set_text(score_text);
	
	set_STATE(active);
	update_collision_box();
    
	anim_pos = 0;
	turn_time = 0;
	score_time = JEWEL_SCORE_TIME;
	set_speed(4); //animation speed in frames per sec
	
	world->add_bonus(get_id());
	
	if(!world->check_bonus(get_id()))
		set_STATE(inactive);
}

GameObject_Jewel::~GameObject_Jewel()
{
	delete (spr_object[active]);
	delete (score_font);
	
	delete(sound);
	delete(sfx_jewel);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Jewel attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Jewel operations:

void GameObject_Jewel::update_STATE()
{	
	if(STATE == inactive)
		return;
	
	if(STATE == captured && score_time > 0)
	{
		score_time -=turn_time;
		return;
	}
	else if (STATE == captured && score_time <= 0)
	{
		set_STATE(inactive);
		return;
	}
		
	if(world->check_collision(player, this))
	{
		set_STATE(captured);
		sound->play(sfx_jewel);
		world->add_score(bonus_value);
		world->disable_bonus(get_id());
	}
	else if ( world->bomb->get_type() == kill_all )
	{
		//check collision with bomb explosion
		if(get_collision_rect()->is_overlapped(world->bomb->get_collision_rect()))
		{
			set_STATE(inactive);
			world->disable_bonus(get_id());
		}
	}
	else if ( world->bullet->get_type() == kill_all )
	{
		//check collision with firing bullet
		if(get_collision_rect()->is_overlapped(world->bullet->get_collision_rect()))
		{
			set_STATE(inactive);
			world->disable_bonus(get_id());
		}
	}
	
}


void GameObject_Jewel::set_STATE(ObjectState state)
{		
	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Jewel::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/jewel/active/collision_box");
	CB.width = cb_element->IntAttribute("width"); 
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Jewel::turn(int time_elapsed)
{
	update_STATE();
	
	if(STATE == inactive)
		return false;	
	
	turn_time = time_elapsed;
				
	anim_pos = 0;
	
	if(STATE == captured)
	{
		y_velocity = -2;
		return move(time_elapsed, false);
	}
	else
		y_velocity += GRAVITY;
		
	return move(time_elapsed);
}

void GameObject_Jewel::show(int view_x, int view_y)
{	
	if(STATE == inactive)
		return;
	
	if(STATE == captured)
	{
		score_font->draw(
			current_position->x  - view_x,
			(current_position->y - spr_object[active]->get_height() +1) - view_y);
		return;
	}
	
	//only render if in window
	int scroll_delta = 0;
	if((world->scroll_y - scroll_delta) > current_position->y ||
	   (world->scroll_y + world->gc->get_height() + scroll_delta) < (current_position->y - spr_object[STATE]->get_height()))
		return;
	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		return;
				
	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;
	
	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x, 
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}

