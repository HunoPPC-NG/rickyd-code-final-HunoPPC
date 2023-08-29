/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include <ctime>
#include <stdlib.h>
#include "precomp.h"
#include "gameobject_walker.h"
#include "gameobject_player.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Walker construction:

GameObject_Walker::GameObject_Walker(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	set_type(kill_player);
	in_pursuit = false;
	TriggerBox = NULL;
	obj_name = object_name;

	spr_object[walk_right] = NULL;
	spr_object[walk_left]  = NULL;
	spr_object[dead]       = NULL;
	spr_object[waiting]    = NULL;

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/walk/right/sprite"))
		spr_object[walk_right] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/walk/right/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/walk/left/sprite"))
		spr_object[walk_left]  = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/walk/left/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/dead/anim/sprite"))
		spr_object[dead]       = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/dead/anim/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/waiting/anim/sprite"))
		spr_object[waiting]    = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/waiting/anim/sprite", world->global_resources);


	if(!spr_object[walk_right])
	{
		std::cerr<< "GameObject_Walker: (FATAL) no matching sprite for walk right found\n";
		exit(-1);
	}
	if(!spr_object[walk_left])
	{
		std::cerr<< "GameObject_Walker: (FATAL) no matching sprite for walk left found\n";
		exit(-1);
	}
	if(!spr_object[dead])
	{
		std::cerr<< "GameObject_Walker: (FATAL) no matching sprite for dead anim found\n";
		exit(-1);
	}

	sfx_dead = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/dead/sample", world->global_resources);
	sfx_dead->set_volume(1.0);

	sound= new RE_SoundSession();

	freeze_time = 0;
	anim_pos = 0;
	triggered = false; //for waiting state

	i_am_dead = false;

	default_speed = 5;
	set_speed(default_speed); //animation speed in frames per sec

	initial_state = 0;
	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");

	object_element->FirstChildElement("position")->QueryIntAttribute("state", &initial_state);

	if(initial_state > 3)
	{
		//add waiting sprite
		if(world->global_resources->file_exists("global/objects/"+obj_name+"/waiting/anim/sprite"))
			spr_object[waiting] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/waiting/anim/sprite", world->global_resources);

		if(!spr_object[waiting])
		{
			std::cerr<< "GameObject_Walker: (FATAL) no matching sprite for waiting anim found\n";
			exit(-1);
		}
		if(initial_state < 6)
			set_type(wall);
		else
			set_type(kill_none);
	}
	set_state(initial_state);

	update_collision_box();

	int bb_x = object_element->FirstChildElement("bounding_box")->IntAttribute("x");
	int bb_y = object_element->FirstChildElement("bounding_box")->IntAttribute("y");
	int bb_width = object_element->FirstChildElement("bounding_box")->IntAttribute("width");
	int bb_height = object_element->FirstChildElement("bounding_box")->IntAttribute("height");

	bounding_box = new RE_Rect(bb_x, bb_y - bb_height, bb_x + bb_width, bb_y);

	def_wait_change_dir_max = WALKER_WAIT_CHANGE_DIR_MAX;
	def_wait_change_dir_min = WALKER_WAIT_CHANGE_DIR_MIN;
	def_wait_change_dir = 0 ;

	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("change_dir_max", &def_wait_change_dir_max);
		param_element->QueryIntAttribute("change_dir", &def_wait_change_dir);
		param_element->QueryIntAttribute("change_dir_min", &def_wait_change_dir_min);
	}

	if(!def_wait_change_dir)
	{
		def_wait_change_dir = def_wait_change_dir_min;
		def_wait_change_dir += abs(def_wait_change_dir_max - def_wait_change_dir_min)/2;
	}

	//check for pursuit|waiting trigger box
	tinyxml2::XMLElement* trigger_element = NULL;
	trigger_element = object_element->FirstChildElement("trigger_box");
	if(trigger_element)
	{
		int tb_x = trigger_element->IntAttribute("x");
		int tb_y = trigger_element->IntAttribute("y");
		int tb_width = trigger_element->IntAttribute("width");
		int tb_height = trigger_element->IntAttribute("height");

		set_trigger_box(tb_x, tb_y, tb_width, tb_height);
	}

	wait_change_dir = def_wait_change_dir;
	turn_time = 0;
	move_time = 0;
}

GameObject_Walker::~GameObject_Walker()
{
	delete(bounding_box);
	if(TriggerBox != NULL)
		delete(TriggerBox);

	delete(spr_object[walk_right]);
	delete(spr_object[walk_left]);
	delete(spr_object[dead]);
	if(spr_object[waiting])
		delete spr_object[waiting];

	delete(sound);
	delete(sfx_dead);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Walker attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Walker operations:
void GameObject_Walker::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Walker::check_trigger()
{
	if( TriggerBox != NULL && world->player->get_type() != kill_none)
	{
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));
	}
	return false;
}

bool GameObject_Walker::in_window(ObjectState state)
{
	if(!spr_object[state])
		return false;

	int scroll_delta = 0;
	if((world->scroll_y - scroll_delta) > current_position->y ||
	   (world->scroll_y + world->gc->get_height() + scroll_delta) < (current_position->y - spr_object[state]->get_height()))
		return false;
	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		return false;

	return true;
}

bool GameObject_Walker::do_random(int chance)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	/* generate a random number(1-100):*/
	int random_num = rand() % 100 + 1;

	//check if random number is less than chance (0-100)
	return ((random_num < chance)? true:false);
}

int GameObject_Walker::get_change_dir_time()
{
	/* initialize random seed: */
	srand ( time(NULL) );

	/* generate a random number(1-9):*/
	int random_num = rand() % 9 + 1;

	//check if random number is even (50% chance)
	//return ((random_num % 2 ) == 0? def_wait_change_dir: def_wait_change_dir_min);

	switch(random_num)
	{
		case 1:
		case 5:
			return def_wait_change_dir_max;
			break;
		case 2:
		case 7:
			return def_wait_change_dir_min;
			break;
		default: //3,4,6,8,9
			return def_wait_change_dir;
			break;
	}
}

bool GameObject_Walker::play_sound()
{
	if(STATE != previous_STATE && sound->is_playing())
	{
		sound->stop();
	}
	else if(STATE == previous_STATE && (sound->is_playing() || STATE == dead))
		return false;

	switch(STATE)
	{
		case dead:
			//store the previous STATE
			set_STATE(STATE, true);
			sound->play(sfx_dead);
			break;
	}

	return true;
}

void GameObject_Walker::update_STATE()
{
	//make sure we are within the current horizontal scroll
	if(STATE == inactive)
		return;

	if(STATE == waiting)
	{
		if(check_trigger())
			triggered = true;
		x_velocity = 0;
		return;
	}

	if(STATE == dead)
	{
		if(current_position->y >= world->map->get_map_height() ||
		   current_position->y - spr_object[STATE]->get_height() >= world->gc->get_height() + world->scroll_y)
		{
			i_am_dead = true;
			set_STATE(inactive);
		}
		else
		{
			if(previous_STATE != dead)
			{
				world->add_score(50);

				if(current_position->x < world->gc->get_width()/2)
					x_velocity = WALKER_VELOCITY;
				else
					x_velocity = -WALKER_VELOCITY;
			}
		}

		//gravity
		y_velocity += GRAVITY;
		return;
	}

	//check for stick collision
	GameObject_Player *player = (GameObject_Player *) world->player;
	if(player->is_stick(get_collision_rect()))
		freeze_time = WALKER_FREEZE_TIME;

	if(freeze_time > 0)
		return;

	//store the previous STATE
	set_STATE(STATE, true);

	//check pursuit trigger
	if( !triggered &&
		!in_pursuit &&
		check_trigger())
	{
		in_pursuit = true;
		if(current_position->x < world->player->current_position->x)
			set_STATE(walk_right);
		else if(current_position->x > world->player->current_position->x)
			set_STATE(walk_left);

		wait_change_dir = get_change_dir_time();
	}

	if( follow_player && wait_change_dir < 0 )
	{
		if(do_random())
		{
			if((x_velocity < 0) &&
				(current_position->x < world->player->current_position->x - 2*world->map->get_tile_width()))
			{
				set_STATE(walk_right);
			}
			else if((x_velocity > 0) &&
				(current_position->x > world->player->current_position->x + 2*world->map->get_tile_width()))
			{
				set_STATE(walk_left);
			}
		}
		wait_change_dir = get_change_dir_time();
	}
	else if(follow_player && x_velocity != 0)
	{
		wait_change_dir -= move_time;
	}
	else if(follow_player) //falling
	{
		if(current_position->x < world->player->current_position->x)
			set_STATE(walk_right);
		else if(current_position->x > world->player->current_position->x)
			set_STATE(walk_left);
	}

	//check collision with bounding_box
	if(!in_pursuit)
	{
		if(current_position->x < bounding_box->Left())
		{

			set_STATE(walk_right);
			wait_change_dir = get_change_dir_time();
		}
		else if(current_position->x > bounding_box->Right())
		{

			set_STATE(walk_left);
			wait_change_dir = get_change_dir_time();
		}
	}


	//check wall collision (we must be within the current horizontal scroll)
	if(collision_left)
	{
		set_STATE(walk_right);
		wait_change_dir = get_change_dir_time();
	}
	else if (collision_right)
	{
		set_STATE(walk_left);
		wait_change_dir = get_change_dir_time();
	}
	else if( STATE == walk_left &&
	         in_window(STATE) &&
	         current_position->x < (world->scroll_x + world->map->get_tile_width()))
	{
		set_STATE(walk_right);
		wait_change_dir = get_change_dir_time();
	}
	else if( STATE == walk_right &&
			 in_window(STATE) &&
			 current_position->x > (world->scroll_x + world->gc->get_width() - world->map->get_tile_width()))
	{
		set_STATE(walk_left);
		wait_change_dir = get_change_dir_time();
	}

	//we are falling
	if(!collision_bottom && y_velocity > 1)
	{
		if(current_position->x < world->player->current_position->x)
			set_STATE(walk_right);
		else if(current_position->x > world->player->current_position->x)
			set_STATE(walk_left);

		x_velocity = 0;
	}

	//we are not falling
	if(collision_bottom)
	{
		switch(STATE)
		{
			case (walk_left):
				x_velocity = -WALKER_VELOCITY;
				break;
			case (walk_right):
				x_velocity = WALKER_VELOCITY;
				break;
		}
	}

	//gravity
	y_velocity += GRAVITY;
}

void GameObject_Walker::deploy(int start_x, int start_y, int state)
{
	if(state >= 0)
	{
		current_position->x = start_x;
		current_position->y = start_y;
		set_state(state);
	}
	else
	{
		current_position->x += start_x;
		current_position->y += start_y;
	}
}

void GameObject_Walker::set_state(int state)
{
	switch(state)
	{
		case 0:
			set_STATE(walk_left);
			follow_player = false;
			break;
		case 1:
			set_STATE(walk_right);
			follow_player = false;
			break;
		case 2:
			set_STATE(walk_left);
			follow_player = true;
			break;
		case 4: //waiting + walk_left (wall)
		case 5: //waiting + walk_right (wall)
		case 6: //waiting (kill_none)
			set_STATE(waiting);
			follow_player = false;
			break;
		case 3:
		default:
			set_STATE(walk_right);
			follow_player = true;
			break;
	}
}

void GameObject_Walker::set_STATE(ObjectState state, bool final)
{
	if(state == dead)
	{
		set_type(kill_none);
		if(previous_STATE != dead)
		{
			anim_pos = 0;
			y_velocity = -8;
		}
	}

	if(state == waiting)
	{
		anim_pos = 0;
		set_speed(15); //animation speed
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Walker::update_collision_box()
{
	if(STATE == inactive)
		return;

	std::string action;

	switch(STATE)
	{
		case walk_right:
		case walk_left:
			action = "walk";
			break;
		case waiting:
			action = "waiting";
			break;
		case dead:
			action = "dead";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Walker::turn(int time_elapsed)
{
	if(STATE == inactive)
		return false;

	if((STATE != dead) && (world->check_collision(kill_all, this) ))
	{
		if(STATE == waiting) //trigger waiting object
			triggered = true;
		else
			set_STATE(dead);
	}

	move_time = time_elapsed;

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(freeze_time > 0)
		freeze_time -= time_elapsed;

	bool ret = false;
	if(STATE != dead)
	{
		int scroll_delta = 1 * world->map->get_tile_height();
		if(((world->scroll_y - scroll_delta) < current_position->y &&
		   (world->scroll_y + world->gc->get_height() + scroll_delta) > (current_position->y - spr_object[STATE]->get_height())) ||
		   abs(world->player->current_position->y - current_position->y) < floor(world->gc->get_height()/2))
		{
			update_STATE();
			//if freeze don't move
			if(freeze_time <= 0)
				ret = move(time_elapsed);
			else
				return ret;
		}
		else
			ret = false;
	}
	else
	{
		update_STATE();
		ret = move(time_elapsed, false);
	}
	//update_collision_box();

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	if(STATE == waiting && spr_object[STATE])
	{
		int anim_length = spr_object[STATE]->get_frame_count();
		if(!triggered)
			anim_pos = 0;
		else if (anim_pos > anim_length-1)
		{
			anim_pos = 0;
			set_type(kill_player);
			set_speed(default_speed);//return to default anim speed
			if(initial_state == 4)
				set_STATE(walk_left);
			else
				set_STATE(walk_right); //initial_state == 5 or 6
		}
	}

	return ret;
}

void GameObject_Walker::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	//only render if in window
	if(!in_window(STATE))
		return;

	play_sound();

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1)
		anim_pos=0;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		 current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
