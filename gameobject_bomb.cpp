/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_bomb.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Bomb construction:

GameObject_Bomb::GameObject_Bomb(World *world) :
	GameObject_Moving(world)
{
	set_type(kill_none);

	spr_object[burning] = new RE_Sprite(world->gc, "global/objects/bomb/active/burning/sprite", world->global_resources);
	spr_object[explosion] = new RE_Sprite(world->gc, "global/objects/bomb/active/explosion/sprite", world->global_resources);

	sfx_deploy = new RE_SoundBuffer("global/objects/bomb/sounds/deploy/sample", world->global_resources);
	sfx_deploy->set_volume(1.0f);

	sfx_explosion = new RE_SoundBuffer("global/objects/bomb/sounds/explosion/sample", world->global_resources);
	sfx_explosion->set_volume(1.0f);

	slide = 0;
	friction = 0;
	tinyxml2::XMLElement* burning_elem = world->global_resources->getElement("global/objects/bomb/active/burning");
	if(burning_elem)
		burning_elem->QueryIntAttribute("slide", &slide);
	if(burning_elem)
		burning_elem->QueryFloatAttribute("friction", &friction);

	sound = new RE_SoundSession();

	STATE = inactive;
	previous_STATE = STATE;
	update_collision_box();
    set_total(MAX_BOMBS);

	anim_pos = 0;
	timer = 0;

	set_speed(ANIM_SPEED);

	current_position->x = 0;
	current_position->y = 0;

	turn_time = 0;
}

GameObject_Bomb::~GameObject_Bomb()
{
	delete(spr_object[burning]);
	delete(spr_object[explosion]);

	delete(sound);
	delete(sfx_deploy);
	delete(sfx_explosion);

}

void GameObject_Bomb::update_STATE()
{
}

//:deploy a bomb
void GameObject_Bomb::deploy(int start_x, int start_y, int start_state)
{
	if(STATE != inactive || bombs_available <= 0)
		return;

	deploy_state = start_state;

	set_STATE(burning);
	timer = BOMB_TIMER;

	current_position->x = start_x - spr_object[burning]->get_width()/2;
	current_position->y = start_y - 4; //elevator velocity

	if(slide)
		switch(deploy_state)
		{
			case -1:
				x_velocity = -slide;
				break;
			case 1:
				x_velocity = slide;
				break;
			default:
				x_velocity = 0;
				break;
		}

	sound->play(sfx_deploy);
}


void GameObject_Bomb::set_STATE(ObjectState state)
{
	if(state == inactive)
	{
		set_type(kill_none);
	}
	else if(state == burning && STATE == inactive)
	{
		set_type(kill_none);
		bombs_available--;
		if(bombs_available < 0)
			bombs_available = 0;
	}
	else if(state == explosion && STATE == burning)
	{
		set_type(kill_all);
		anim_pos = 0;
		turn_time = 1;
		x_velocity = 0;
		y_velocity = 0;

		//explosion (center)
		current_position->x -= (spr_object[explosion]->get_width() - spr_object[burning]->get_width())/2;
		current_position->y += (spr_object[explosion]->get_height() - spr_object[burning]->get_height())/2;
	}

	previous_STATE = STATE;
	STATE = state;

	update_collision_box();
}


int GameObject_Bomb::get_total()
{
	return bombs_available;
}

void GameObject_Bomb::set_total(int bombs)
{
	bombs_available = bombs;
	if(bombs_available > MAX_BOMBS)
		bombs_available = MAX_BOMBS;
}

void GameObject_Bomb::update_collision_box()
{
	std::string action;

	switch(STATE)
	{
		case burning:
			action = "burning";
			break;
		default:
			action = "explosion";
			break;
	}
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/bomb/active/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Bomb::turn(int time_elapsed)
{
	//update_STATE();

	if(STATE == inactive)
		return false;

	if(STATE != explosion)
	{
		y_velocity += GRAVITY;
		if(slide && STATE == burning)
			switch(deploy_state)
			{
				case -1:
					x_velocity += friction;
					if(x_velocity >= 0)
						x_velocity = 0;
					break;
				case 1:
					x_velocity -= friction;
					if(x_velocity <= 0)
						x_velocity = 0;
					break;
				default:
					x_velocity = 0;
					break;
			}
	}


	if((STATE == burning) && (turn_time < speed))
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(turn_time == 0) //rotate on turn start
	{
		anim_pos++;
		if(STATE == burning)
		{

			timer--;
			if(timer <= 0)
			{
				set_STATE(explosion);
				sound->play(sfx_explosion);
			}
		}
	}

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1)
	{
		anim_pos=0;
		if(STATE == explosion)
			set_STATE(inactive);
	}

	bool ret = move(time_elapsed);

	//std::cout << "bomb: deploy_state:"<<deploy_state<<" x_velocity="<<x_velocity<<"\n";
	return ret;
}

void GameObject_Bomb::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	//only render if in window
	int scroll_delta = 0;
	if((world->scroll_y - scroll_delta) > current_position->y ||
	   (world->scroll_y + world->gc->get_height() + scroll_delta) < (current_position->y - spr_object[STATE]->get_height()))
		return;
	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		return;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
