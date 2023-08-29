/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include <ctime>
#include "precomp.h"
#include "gameobject_climber.h"
#include "gameobject_player.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Climber construction:

GameObject_Climber::GameObject_Climber(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	set_type(kill_player);
	in_pursuit = false;
	TriggerBox = NULL;
	CB.stairs_dx = 4;
	obj_name = object_name;

	spr_object[walk_right] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/walk/right/sprite", world->global_resources);
	spr_object[walk_left]  = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/walk/left/sprite", world->global_resources);
	spr_object[climb]      = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/climb/stairs/sprite", world->global_resources);
	spr_object[dead]       = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/dead/anim/sprite", world->global_resources);

	sfx_dead = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/dead/sample", world->global_resources);
	sfx_dead->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;
	freeze_time = 0;
	i_am_dead = false;
	//got_powerup = false;

	set_speed(5); //animation speed in frames per sec

	current_position->x = object_element->FirstChildElement("position")->IntAttribute("x");
	current_position->y = object_element->FirstChildElement("position")->IntAttribute("y");
	int state =  object_element->FirstChildElement("position")->IntAttribute("state");
	set_state(state);

	initial_scroll_x = abs(current_position->x / world->gc->get_width())*world->gc->get_width();
	//std::cerr <<"Initial scroll x="<<initial_scroll_x<<"\n";
	follow_delta_x = 270;
	follow_delta_y = 30;
	def_wait_change_dir_max = CLIMBER_WAIT_CHANGE_DIR_MAX;
	def_wait_change_dir_min = CLIMBER_WAIT_CHANGE_DIR_MIN;
	def_wait_change_dir = 0 ;
	check_fall = 0;

	if(follow_player)
	{
		//check for parameters
		tinyxml2::XMLElement* param_element = NULL;
		param_element = object_element->FirstChildElement("parameters");
		if(param_element)
		{
			param_element->QueryIntAttribute("follow_delta_x", &follow_delta_x);
			param_element->QueryIntAttribute("follow_delta_y", &follow_delta_y);
			param_element->QueryIntAttribute("change_dir_max", &def_wait_change_dir_max);
			param_element->QueryIntAttribute("change_dir", &def_wait_change_dir);
			param_element->QueryIntAttribute("change_dir_min", &def_wait_change_dir_min);
			param_element->QueryIntAttribute("check_fall", &check_fall);
		}

		if(!def_wait_change_dir)
		{
			def_wait_change_dir = def_wait_change_dir_min;
			def_wait_change_dir += abs(def_wait_change_dir_max - def_wait_change_dir_min)/2;

		}
		//check for pursuit trigger box
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
	}

	wait_change_dir = def_wait_change_dir;

	//query stairs_x
	tinyxml2::XMLElement* stairs_element = NULL;
	for(stairs_element = object_element->FirstChildElement("stairs"); stairs_element != NULL; stairs_element = stairs_element->NextSiblingElement("stairs"))
	{
		int sx = 0; //default no stairs
		stairs_element->QueryIntAttribute("x", &sx);

		if(sx > 0)
			stairs_x.push_back(sx);
	}


	update_collision_box();

	int bb_x = object_element->FirstChildElement("bounding_box")->IntAttribute("x");
	int bb_y = object_element->FirstChildElement("bounding_box")->IntAttribute("y");
	int bb_width = object_element->FirstChildElement("bounding_box")->IntAttribute("width");
	int bb_height = object_element->FirstChildElement("bounding_box")->IntAttribute("height");

	bounding_box = new RE_Rect(bb_x, bb_y - bb_height, bb_x + bb_width, bb_y);

	turn_time = 0;
	move_time = 0;
    stored_velocity = 0;
}

GameObject_Climber::~GameObject_Climber()
{
	stairs_x.clear();

	delete(bounding_box);
	if(TriggerBox != NULL)
		delete(TriggerBox);

	delete(spr_object[walk_right]);
	delete(spr_object[walk_left]);
	delete(spr_object[climb]);
	delete(spr_object[dead]);

	delete(sound);
	delete(sfx_dead);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Climber attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Climber operations:
void GameObject_Climber::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

bool GameObject_Climber::check_trigger()
{
	if( TriggerBox != NULL && world->player->get_type() != kill_none)
		return(TriggerBox->is_overlapped(world->player->get_collision_rect()));

	return false;
}

bool GameObject_Climber::in_window(ObjectState state)
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

int GameObject_Climber::get_change_dir_time()
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

bool GameObject_Climber::do_random(int chance)
{
	/* initialize random seed: */
	srand ( time(NULL) );

	/* generate a random number(1-100):*/
	int random_num = rand() % 100 + 1;

	//check if random number is less than chance (0-100)
	return ((random_num < chance)? true:false);
}

bool GameObject_Climber::play_sound()
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

void GameObject_Climber::update_STATE()
{
	if(STATE == inactive)
		return;

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
					x_velocity = CLIMBER_VELOCITY;
				else
					x_velocity = -CLIMBER_VELOCITY;
			}
		}

		//gravity
		y_velocity += GRAVITY;
		return;
	}

	//check for stick collision
	GameObject_Player *player = (GameObject_Player *) world->player;
	if(player->is_stick(get_collision_rect()))
		freeze_time = CLIMBER_FREEZE_TIME;

	if(freeze_time > 0)
		return;


	//store the previous STATE
	set_STATE(STATE, true);

	//check pursuit trigger
	if(check_trigger() && !in_pursuit)
	{
		in_pursuit = true;
		if(STATE != climb)
		{
			if(current_position->x < world->player->current_position->x)
				set_STATE(walk_right);
			else if(current_position->x > world->player->current_position->x)
				set_STATE(walk_left);

			wait_change_dir = get_change_dir_time();
		}
	}
	//vertical movement
	if(STATE == climb)
	{
        if(y_velocity == 0)
        {
            y_velocity = stored_velocity;
        }

		if( follow_player )
		{
			if(abs(current_position->y - world->player->current_position->y) < follow_delta_y &&
			   abs(current_position->x - world->player->current_position->x) < follow_delta_x)
			{
				if(current_position->x > world->player->current_position->x)
					x_velocity = -CLIMBER_VELOCITY;
				else
					x_velocity = CLIMBER_VELOCITY;
			}

		}

        if(follow_vertical ||
          (follow_player &&
            abs(current_position->y - world->player->current_position->y) < follow_delta_y &&
			abs(current_position->x - world->player->current_position->x) < follow_delta_x))
        {
            if( y_velocity > 0 &&
				world->player->current_position->y <= current_position->y )
				y_velocity = 0;
			else if(y_velocity < 0 &&
					world->player->current_position->y >= current_position->y)
				y_velocity = 0;

			if(current_position->y > world->player->current_position->y + CLIMBER_VELOCITY)
				y_velocity = -CLIMBER_VELOCITY;
			else if (current_position->y < world->player->current_position->y - CLIMBER_VELOCITY)
				y_velocity = CLIMBER_VELOCITY;
        }

		if(!in_pursuit)
		{
			//check bounding box
			if(current_position->y >= bounding_box->Bottom())
			{
				if(current_position->y > world->player->current_position->y)
					y_velocity = -CLIMBER_VELOCITY; //go up
				else
				{
					y_velocity = 0;
					if(current_position->x > world->player->current_position->x)
						x_velocity = -CLIMBER_VELOCITY;
					else
						x_velocity = CLIMBER_VELOCITY;
				}
			}
			else if(current_position->y <= bounding_box->Top())
			{
				if(current_position->y < world->player->current_position->y)
					y_velocity = CLIMBER_VELOCITY; //go down
				else
				{
					y_velocity = 0;
					if(current_position->x > world->player->current_position->x)
						x_velocity = -CLIMBER_VELOCITY;
					else
						x_velocity = CLIMBER_VELOCITY;
				}
			}
		}
		//check collisions
		if(!collision_stairs)
		{
			if(!collision_bottom) //falling
			{
				x_velocity = 0;
				if(current_position->x < world->player->current_position->x)
					set_STATE(walk_right);
				else
					set_STATE(walk_left);
			}
			else if( current_position->y < world->player->current_position->y &&
				do_random() )
			{
				//go down again
				y_velocity = CLIMBER_VELOCITY;
			}
			else
			{
				if(current_position->x < world->player->current_position->x)
					set_STATE(walk_right);
				else
					set_STATE(walk_left);
			}
		}
		else if(collision_bottom)
		{
			if(current_position->y > world->player->current_position->y &&
				do_random() )
			{
				//go up again
				y_velocity = -CLIMBER_VELOCITY;
			}
			else
			{
				if(current_position->x < world->player->current_position->x)
					set_STATE(walk_right);
				else
					set_STATE(walk_left);
			}
		}
        else
        {
            RE_Point my_point;
            my_point.x = current_position->x + floor(get_width()/2);
            my_point.y = current_position->y;
            if(y_velocity < 0)
                my_point.y -= get_height() + 1;
            else
                 my_point.y += 1;
            GameObject* other = world->check_point_collision(kill_player, &my_point, this);
            if(other && other->on_climb)
            {
                if(y_velocity != 0)
                    stored_velocity = y_velocity;
                y_velocity = 0;
            }

            if(y_velocity == 0)
            {
            	//check bottom tile
            	my_point.y = current_position->y + 1;
            	switch(world->map->get_tile_type(&my_point))
            	{
            		case Map::tile_wall:
            		case Map::tile_platform:
            			if(current_position->x < world->player->current_position->x)
            			{
							set_STATE(walk_right);
							x_velocity = CLIMBER_VELOCITY;
						}
						else
						{
							set_STATE(walk_left);
							x_velocity = -CLIMBER_VELOCITY;
						}
            			break;
            	}
            }
        }
	}

	//horizontal movement
	if( STATE != climb)
	{
		if( follow_player && wait_change_dir < 0 )
		{
			if(do_random())
			{
				//follow player
				if((x_velocity < 0) &&
					(current_position->x < world->player->current_position->x - 2*world->map->get_tile_width()))
				{
					set_STATE(walk_right);
				}
				else if ((x_velocity > 0) &&
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

		//check wall collisions
		if( collision_left )
		{
			set_STATE(walk_right);
			wait_change_dir = get_change_dir_time();
		}
		else if( collision_right )
		{
			set_STATE(walk_left);
			wait_change_dir = get_change_dir_time();
		}
		//else if(current_position->x < (world->scroll_x + world->map->get_tile_width()) && STATE == walk_left)
		else if(current_position->x < initial_scroll_x)
		{
			set_STATE(walk_right);
			wait_change_dir = get_change_dir_time();
		}
		//else if(current_position->x > (world->scroll_x + world->gc->get_width() - world->map->get_tile_width()) && STATE == walk_right)
		else if(current_position->x > initial_scroll_x + world->gc->get_width())
		{
			set_STATE(walk_left);
			wait_change_dir = get_change_dir_time();
		}

		//check if move causes fall
		if(check_fall && !can_walk())
		{
			if(STATE == walk_left)
				set_STATE(walk_right);
			else if (STATE == walk_right)
				set_STATE(walk_left);
		}

		if(!collision_bottom && y_velocity > 1)
		{
			x_velocity = 0;
		}

		//we are not falling
		if(collision_bottom)
		{
			switch(STATE)
			{
				case (walk_left):
					x_velocity = -CLIMBER_VELOCITY;
					break;
				case (walk_right):
					x_velocity = CLIMBER_VELOCITY;
					break;
			}
		}

		//check for stairs
		for(int i=0; i<stairs_x.size(); i++)
		{
			if( (current_position->x + CB.dx + CB.stairs_dx ) < (stairs_x[i] + world->map->get_tile_width()) &&
				 (current_position->x + CB.dx + CB.width - CB.stairs_dx) > (stairs_x[i] + world->map->get_tile_width()) &&
				 follow_player &&
				 do_random(70) )
			{
				if(collision_stairs_bottom &&
				   current_position->y < world->player->current_position->y &&
				   (in_pursuit || current_position->y < bounding_box->Bottom()))
				{
					//go down
					x_velocity = 0;
					y_velocity = CLIMBER_VELOCITY;
					current_position->x = stairs_x[i] + world->map->get_tile_width() - CB.dx - floor(CB.width/2);
					set_STATE(climb);
				}
				else if(collision_stairs &&
						current_position->y > world->player->current_position->y &&
						(in_pursuit || current_position->y > bounding_box->Top()))
				{
					//go up
					x_velocity = 0;
					y_velocity = -CLIMBER_VELOCITY;
					current_position->x = stairs_x[i] + world->map->get_tile_width() - CB.dx - floor(CB.width/2);
					set_STATE(climb);
				}
			}
		}
	}

	if(on_elevator)
	{
		anim_pos = spr_object[STATE]->get_frame_count(); //will reset to 0
		x_velocity = 0;
		on_elevator = false; //reset
	}

	//gravity
	if(!on_climb)
		y_velocity += GRAVITY;
}

void GameObject_Climber::deploy(int start_x, int start_y, int state)
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

void GameObject_Climber::set_state(int state)
{
	switch(state)
	{
		case 0:
			set_STATE(walk_left);
			follow_player = false;
			follow_vertical = false;
			break;
		case 1:
			set_STATE(walk_right);
			follow_player = false;
			follow_vertical = false;
			break;
		case 2:
			set_STATE(walk_left);
			follow_player = true;
			follow_vertical = false;
        case 3:
            set_STATE(walk_right);
			follow_player = true;
			follow_vertical = false;
			break;
		case 4:
			set_STATE(walk_left);
			follow_player = true;
            follow_vertical = true;
			break;
		case 5:
			set_STATE(climb);
			follow_player = true;
            follow_vertical = true;
            collision_stairs = true;
			break;
        default:
            set_STATE(walk_right);
			follow_player = true;
            follow_vertical = true;
			break;
	}
}

void GameObject_Climber::set_STATE(ObjectState state, bool final)
{
	on_climb = false;

	if(state == dead)
	{
		set_type(kill_none);
		if(previous_STATE != dead)
		{
			anim_pos = 0;
			y_velocity = -8;
		}
	}

	if(state == climb)
		on_climb = true;

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Climber::update_collision_box()
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
		case climb:
			action = "climb";
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

bool GameObject_Climber::turn(int time_elapsed)
{
	if(STATE == inactive)
		return false;

	if((STATE != dead) && (world->check_collision(kill_all, this) ))
	{
		set_STATE(dead);
	}

	turn_time = time_elapsed;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	bool ret = false;

	if(freeze_time > 0)
		freeze_time -= time_elapsed;

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

	if(move_time == 0) //rotate on turn start
		anim_pos++;

	return ret;
}

void GameObject_Climber::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	//only render if in window
	if(!in_window(STATE))
		return;

	play_sound();

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		 current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
