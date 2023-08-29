/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_player.h"
#include "gameobject_bullet.h"
#include "tinyxml2.h"

#include "libHJWSDL2/HunoJoyWrapper.h"
//
// GameObject_Player construction:

GameObject_Player::GameObject_Player(World *world) :
	GameObject_Moving(world)
{
	set_type(player);

	spr_object[overbike_right]   = NULL;
	spr_object[overbike_left]    = NULL;

	sfx_overbike = NULL;

	spr_object[walk_right]   = new RE_Sprite(world->gc, "global/objects/player/walk/right/sprite", world->global_resources);
	spr_object[walk_left]    = new RE_Sprite(world->gc, "global/objects/player/walk/left/sprite", world->global_resources);
	spr_object[jump_right]   = new RE_Sprite(world->gc, "global/objects/player/jump/right/sprite", world->global_resources);
	spr_object[jump_left]    = new RE_Sprite(world->gc, "global/objects/player/jump/left/sprite", world->global_resources);
	spr_object[climb]        = new RE_Sprite(world->gc, "global/objects/player/climb/stairs/sprite", world->global_resources);
	spr_object[climb_stop]   = new RE_Sprite(world->gc, "global/objects/player/climb/stop/sprite", world->global_resources);
	spr_object[stand_right]  = new RE_Sprite(world->gc, "global/objects/player/stand/right/sprite", world->global_resources);
	spr_object[stand_left]   = new RE_Sprite(world->gc, "global/objects/player/stand/left/sprite", world->global_resources);
	spr_object[crawl_right]  = new RE_Sprite(world->gc, "global/objects/player/crawl/right/sprite", world->global_resources);
	spr_object[crawl_left]   = new RE_Sprite(world->gc, "global/objects/player/crawl/left/sprite", world->global_resources);
	spr_object[duck_right]   = new RE_Sprite(world->gc, "global/objects/player/duck/right/sprite", world->global_resources);
	spr_object[duck_left]    = new RE_Sprite(world->gc, "global/objects/player/duck/left/sprite", world->global_resources);
	spr_object[shoot_right]  = new RE_Sprite(world->gc, "global/objects/player/shoot/right/sprite", world->global_resources);
	spr_object[shoot_left]   = new RE_Sprite(world->gc, "global/objects/player/shoot/left/sprite", world->global_resources);
	spr_object[hammer_right] = new RE_Sprite(world->gc, "global/objects/player/hammer/right/sprite", world->global_resources);
	spr_object[hammer_left]  = new RE_Sprite(world->gc, "global/objects/player/hammer/left/sprite", world->global_resources);
	spr_object[dead]         = new RE_Sprite(world->gc, "global/objects/player/dead/anim/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/player/overbike/right/sprite"))
		spr_object[overbike_right] = new RE_Sprite(world->gc, "global/objects/player/overbike/right/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/player/overbike/left/sprite"))
		spr_object[overbike_left] = new RE_Sprite(world->gc, "global/objects/player/overbike/left/sprite", world->global_resources);


	if(world->global_resources->file_exists("global/objects/player/sounds/overbike/sample"))
	{
		sfx_overbike = new RE_SoundBuffer("global/objects/player/sounds/overbike/sample", world->global_resources);
		sfx_overbike->set_volume(1.0);
	}

	sfx_walk = new RE_SoundBuffer("global/objects/player/sounds/walk/sample", world->global_resources);
	sfx_walk->set_volume(1.0);

	sfx_crawl = new RE_SoundBuffer("global/objects/player/sounds/crawl/sample", world->global_resources);
	sfx_crawl->set_volume(1.0);

	sfx_jump = new RE_SoundBuffer("global/objects/player/sounds/jump/sample", world->global_resources);
	sfx_jump->set_volume(1.0);

	sfx_dead = new RE_SoundBuffer("global/objects/player/sounds/dead/sample", world->global_resources);
	sfx_dead->set_volume(1.0);

	sound= new RE_SoundSession();

	anim_pos = 0;
	scroll_mode = 1; //rick1 mode by default
	stick_time = 0;

	i_am_dead = false;
	//got_powerup = false;

	set_speed(8); //animation speed in frames per sec

	current_position->x = world->map->get_player_x(world->location_index);
	current_position->y = world->map->get_player_y(world->location_index);
	int state = world->map->get_player_state(world->location_index);

	set_state(state);
	update_collision_box();

	tinyxml2::XMLElement* stand_cb_element = world->global_resources->getElement("global/objects/player/stand/collision_box");
	SB.width = stand_cb_element->IntAttribute("width");
    SB.height = stand_cb_element->IntAttribute("height");
    SB.dx = stand_cb_element->IntAttribute("dx");
    SB.dx += (spr_object[duck_right]->get_width() - spr_object[stand_right]->get_width())/2;
	SB.dy = stand_cb_element->IntAttribute("dy");

	//check for player parameters
	tinyxml2::XMLElement* player_element = NULL;
	player_element = world->global_resources->getElement("global/objects/player");
	tinyxml2::XMLElement* param_element = NULL;
	param_element = player_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("scroll_mode", &scroll_mode);
	}

    turn_time = 0;
}

GameObject_Player::~GameObject_Player()
{
	delete(spr_object[walk_right]);
	delete(spr_object[walk_left]);
	delete(spr_object[jump_right]);
	delete(spr_object[jump_left]);
	delete(spr_object[climb]);
	delete(spr_object[climb_stop]);
	delete(spr_object[stand_right]);
	delete(spr_object[stand_left]);
	delete(spr_object[crawl_right]);
	delete(spr_object[crawl_left]);
	delete(spr_object[duck_right]);
	delete(spr_object[duck_left]);
	delete(spr_object[shoot_right]);
	delete(spr_object[shoot_left]);
	delete(spr_object[hammer_right]);
	delete(spr_object[hammer_left]);
	delete(spr_object[dead]);

	if(spr_object[overbike_right])
		delete(spr_object[overbike_right]);

	if(spr_object[overbike_left])
		delete(spr_object[overbike_left]);

	delete(sound);
	if(sfx_overbike)
		delete(sfx_overbike);
	delete(sfx_walk);
	delete(sfx_jump);
	delete(sfx_crawl);
	delete(sfx_dead);
}
//
// GameObject_Player attributes:

//
// GameObject_Player operations:
bool GameObject_Player::is_on_jump_platform()
{
	if(world->check_collision(jump_platform, this))
		return true;

	return false;
}

bool GameObject_Player::play_sound()
{

	if(STATE != previous_STATE && sound->is_playing())
	{
		sound->stop();
	}
	else if(STATE == previous_STATE && (sound->is_playing() || STATE == dead))
		return false;

	//sound effects
	switch(STATE)
	{
		case overbike_right:
		case overbike_left:
			if(turn_time == 0) //animation fps
			{
				if(sfx_overbike)
					sound->play(sfx_overbike);
			}
			break;
		case walk_right:
		case walk_left:
			if(turn_time == 0) //animation fps
			{
				sound->play(sfx_walk);
			}
			break;
		case crawl_right:
		case crawl_left:
			if(turn_time == 0)
			{
				sound->play(sfx_crawl);
			}
			break;
		case jump_right:
		case jump_left:
			if(y_velocity < -(PLAYER_JUMP_VELOCITY -1))
			{
				sound->play(sfx_jump);
			}
			break;
		case dead:
			//store the previous STATE
			set_STATE(STATE, true);
			sound->play(sfx_dead);
			break;
	}

	return true;
}

void GameObject_Player::update_STATE()
{
	//std::cout <<"left:"<< world->events->keys[RE_Events::KEY_LEFT] <<"right:"<<world->events->keys[RE_Events::KEY_RIGHT]<<"down:"<<world->events->keys[RE_Events::KEY_DOWN]<<"action:"<<world->events->keys[RE_Events::KEY_ACTION]<<"\n";
	switch(STATE)
	{
			case inactive:
				return;

			case dead:
				if((current_position->y - spr_object[STATE]->get_height()) > (world->gc->get_height() + world->scroll_y))
				{
					i_am_dead = true;
					set_STATE(inactive);
				}
				else
				{
					if(previous_STATE != dead)
					{
						set_type(kill_none); //disables triggers while dying
						if(current_position->x < (world->gc->get_width()/2))
						{
							x_velocity = PLAYER_VELOCITY;
						}
						else
							x_velocity = -PLAYER_VELOCITY;
					}
				}
				//gravity
				y_velocity += GRAVITY;
				return;

			case climb:
			case climb_stop:
				//store the previous STATE
				set_STATE(STATE, true);
				if(!collision_stairs)
				{
					set_STATE(jump_left);
				}
				else if(!world->events->keys[RE_Events::KEY_UP] &&
						!world->events->keys[RE_Events::KEY_DOWN] &&
						!world->events->keys[RE_Events::KEY_LEFT] &&
						!world->events->keys[RE_Events::KEY_RIGHT])
				{
					y_velocity = 0;
					set_STATE(climb_stop);
				}
				break;

			case crawl_left:
			case crawl_right:
			case duck_left:
			case duck_right:
				//store the previous STATE
				set_STATE(STATE, true);
				if(!world->events->keys[RE_Events::KEY_DOWN] && can_stand(SB))
				{
					switch(STATE)
					{
						case crawl_left:
						case duck_left:
							set_STATE(stand_left);
							break;
						case crawl_right:
						case duck_right:
							set_STATE(stand_right);
							break;
					}
				}
				break;

			case overbike_right:
				y_velocity = 0;
				x_velocity = 0;
				break;

			case overbike_left:
				y_velocity = 0;
				x_velocity = 0;
				break;

			default:
				//store the previous STATE
				set_STATE(STATE, true);
				break;
	}

	//we are falling
	if( STATE != overbike_left &&
		STATE != overbike_right &&
		!collision_bottom &&
		!on_climb &&
		y_velocity > 1)
	{
		switch(STATE)
		{
			case crawl_left:
			case stand_left:
			case walk_left:
			case hammer_left:
				set_STATE(jump_left);
				break;
			case crawl_right:
			case stand_right:
			case walk_right:
			case hammer_right:
				set_STATE(jump_right);
				break;
		}
	}

	//we are not falling
	if(collision_bottom)
	{
		switch(STATE)
		{
			case jump_right:
				set_STATE(stand_right);
				break;
			case jump_left:
			case climb:
			case climb_stop:
				set_STATE(stand_left);
				break;
		}
	}

	//check movement keys (horizontal)
	if(world->events->keys[RE_Events::KEY_LEFT])
	{
		x_velocity = -PLAYER_VELOCITY;

		switch(STATE)
		{
			case jump_right:
			case jump_left:
				set_STATE(jump_left);
				break;
			case crawl_right:
			case crawl_left:
			case duck_right:
			case duck_left:
				set_STATE(crawl_left);
				break;
			case climb:
			case climb_stop:
				if(collision_stairs)
					set_STATE(climb);
				else
					set_STATE(jump_left);
				break;
			case overbike_right:
				set_STATE(overbike_left);
				x_velocity = -OVERBIKE_VELOCITY;
				break;
			case overbike_left:
				x_velocity = -OVERBIKE_VELOCITY;
				break;
			default:
				set_STATE(walk_left);
				break;
		}
	}
	else if(world->events->keys[RE_Events::KEY_RIGHT])
	{
		x_velocity = PLAYER_VELOCITY;

		switch(STATE)
		{
			case jump_right:
			case jump_left:
				set_STATE(jump_right);
				break;
			case crawl_right:
			case crawl_left:
			case duck_right:
			case duck_left:
				set_STATE(crawl_right);
				break;
			case climb:
			case climb_stop:
				if(collision_stairs)
					set_STATE(climb);
				else
					set_STATE(jump_right);
				break;
			case overbike_right:
				x_velocity = OVERBIKE_VELOCITY;
				break;
			case overbike_left:
				set_STATE(overbike_right);
				x_velocity = OVERBIKE_VELOCITY;
				break;
			default:
				set_STATE(walk_right);
				break;
		}

	}
	else if(!world->events->keys[RE_Events::KEY_LEFT] && !world->events->keys[RE_Events::KEY_RIGHT])
	{
		x_velocity = 0;

		switch(STATE)
		{
			case walk_left:
			case stand_left:
				set_STATE(stand_left);
				break;
			case crawl_left:
				set_STATE(duck_left);
				break;
			case walk_right:
			case stand_right:
				set_STATE(stand_right);
				break;
			case crawl_right:
				set_STATE(duck_right);
				break;
			case shoot_left:
				if(!world->events->keys[RE_Events::KEY_UP] || !world->events->keys[RE_Events::KEY_ACTION])
					set_STATE(stand_left);
				break;
			case shoot_right:
				if(!world->events->keys[RE_Events::KEY_UP] || !world->events->keys[RE_Events::KEY_ACTION])
					set_STATE(stand_right);
				break;
			case hammer_left:
				if(!world->events->keys[RE_Events::KEY_LEFT] || !world->events->keys[RE_Events::KEY_ACTION])
					set_STATE(stand_left);
				break;
			case hammer_right:
				if(!world->events->keys[RE_Events::KEY_RIGHT] || !world->events->keys[RE_Events::KEY_ACTION])
					set_STATE(stand_right);
				break;
		}
	}

	//action key pressed
	if(world->events->keys[RE_Events::KEY_ACTION])
	{
		//shoot action
		if(world->events->keys[RE_Events::KEY_UP] &&
		     STATE != jump_right &&
		     STATE != jump_left &&
		     STATE != climb &&
		     STATE != crawl_left &&
		     STATE != crawl_right &&
		     STATE != duck_left &&
		     STATE != duck_right &&
		     STATE != overbike_right &&
		     STATE != overbike_left)
		{
			switch(STATE)
			{
				case stand_right:
				case walk_right:
				case crawl_right:
				case duck_right:
				case shoot_right:
					set_STATE(shoot_right);
					x_velocity = 0;
					break;
				default:
					set_STATE(shoot_left);
					x_velocity = 0;
					break;
			}
		}  //set bomb
		else if(world->events->keys[RE_Events::KEY_DOWN] &&
			STATE != jump_right &&
			STATE != jump_left &&
			STATE != climb &&
			STATE != climb_stop &&
			STATE != overbike_right &&
		    STATE != overbike_left)
		{
			x_velocity = 0;
			int bx = current_position->x + spr_object[STATE]->get_width()/2 ;

			if(world->events->keys[RE_Events::KEY_LEFT])
				world->bomb->deploy(bx, current_position->y, -1); //slide left
			else if (world->events->keys[RE_Events::KEY_RIGHT])
				world->bomb->deploy(bx, current_position->y, 1); //slide right
			else
				world->bomb->deploy(bx, current_position->y, 0); //don't slide
		}//hammer left or slider bomb
		else if(world->events->keys[RE_Events::KEY_LEFT] &&
		     STATE != jump_right &&
		     STATE != jump_left &&
		     STATE != climb &&
			 STATE != climb_stop &&
		     STATE != crawl_right &&
		     STATE != crawl_left &&
		     STATE != duck_left &&
		     STATE != duck_right &&
		     STATE != overbike_right &&
		     STATE != overbike_left)
		{
			//std::cout <<"down:"<< world->events->keys[RE_Events::KEY_DOWN] <<"\n";
			x_velocity = 0;
			if(world->events->keys[RE_Events::KEY_DOWN])
			{
				int bx = current_position->x + spr_object[STATE]->get_width()/2 ;
				world->bomb->deploy(bx, current_position->y, -1); //slide left)
			}
			else
				set_STATE(hammer_left);
		}//hammer right or slider bomb
		else if(world->events->keys[RE_Events::KEY_RIGHT] &&
		     STATE != jump_right &&
		     STATE != jump_left &&
		     STATE != climb &&
			 STATE != climb_stop &&
		     STATE != crawl_right &&
		     STATE != crawl_left &&
		     STATE != duck_left &&
		     STATE != duck_right &&
		     STATE != overbike_right &&
		     STATE != overbike_left)
		{
			//std::cout <<"down:"<< world->events->keys[RE_Events::KEY_DOWN] <<"\n";
			x_velocity = 0;
			if(world->events->keys[RE_Events::KEY_DOWN])
			{
				int bx = current_position->x + spr_object[STATE]->get_width()/2 ;
				world->bomb->deploy(bx, current_position->y, 1); //slide right)
			}
			else
				set_STATE(hammer_right);
		}
		else if(STATE == overbike_right) //shoot right
		{
			int dir = 3;
			GameObject_Bullet *bullet = (GameObject_Bullet *) world->bullet;
			int bx = current_position->x + spr_object[STATE]->get_width();
			int by = current_position->y - floor(spr_object[STATE]->get_height()/2);

			bullet->deploy(bx, by, dir);
		}
		else if(STATE == overbike_left) //shoot left
		{
			int dir = 2; //left
			GameObject_Bullet *bullet = (GameObject_Bullet *) world->bullet;
			int bx = current_position->x;
			int by = current_position->y - floor(spr_object[STATE]->get_height()/2);

			bullet->deploy(bx, by, dir);
		}

		//move overbike up/down even if action key pressed
		if( STATE == overbike_right ||
			STATE == overbike_left )
		{
			if(world->events->keys[RE_Events::KEY_UP])
				y_velocity = -OVERBIKE_VELOCITY;
			else if( world->events->keys[RE_Events::KEY_DOWN])
				y_velocity = OVERBIKE_VELOCITY;
		}

	}
	else if(world->events->keys[RE_Events::KEY_UP]) //no action key
	{
		if( STATE == overbike_left ||
			STATE == overbike_right )
		{
			y_velocity = -OVERBIKE_VELOCITY;
		}
		else if(collision_stairs)
		{
			y_velocity = -PLAYER_VELOCITY;
			set_STATE(climb);
		}
		else if(collision_bottom &&
				STATE != crawl_right &&
				STATE != crawl_left  &&
				STATE != duck_right  &&
				STATE != duck_left) //jump
		{
			y_velocity = -PLAYER_JUMP_VELOCITY;
			if(is_on_jump_platform())
				y_velocity = -1.35 * PLAYER_JUMP_VELOCITY; //go higher

			on_jump = true;
			switch(STATE)
			{
				case walk_right:
				case stand_right:
				case crawl_right:
				case duck_right:
				case shoot_right:
					set_STATE(jump_right);
					break;
				case walk_left:
				case stand_left:
				case crawl_left:
				case duck_left:
				case shoot_left:
					set_STATE(jump_left);
					break;
			}
		}
	}
	else if(world->events->keys[RE_Events::KEY_DOWN]) //no action key
	{
		if( STATE == overbike_left ||
			STATE == overbike_right )
		{
			y_velocity = OVERBIKE_VELOCITY;
		}
		else if( ( collision_stairs_bottom &&
			  STATE != crawl_left &&
			  STATE != crawl_right ) ||
		   	 ( collision_stairs  &&
		   	   !collision_bottom ) )
		{
			y_velocity = PLAYER_VELOCITY;
			set_STATE(climb);
		}

		if(collision_bottom)
		{
			switch(STATE)
			{
				case walk_right:
				case shoot_right:
				case stand_right:
				case crawl_right:
				case duck_right:
					if(!x_velocity)
						set_STATE(duck_right);
					else
						set_STATE(crawl_right);
					break;
				case walk_left:
				case shoot_left:
				case stand_left:
				case crawl_left:
				case duck_left:
					if(!x_velocity)
						set_STATE(duck_left);
					else
						set_STATE(crawl_left);
					break;
			}
		}
	}

	//gravity
	if( !on_climb &&
		STATE != overbike_right &&
		STATE != overbike_left )
		y_velocity += GRAVITY;
}

void GameObject_Player::deploy(int start_x, int start_y, int state)
{
	if(state >= 0)
	{
		current_position->x = start_x;
		current_position->y = start_y;
		set_state(state);
	}
	else //state < 0 => increment on current position
	{
		current_position->x += start_x;
		current_position->y += start_y;
	}
}

void GameObject_Player::set_state(int state)
{
	switch(state)
	{
		case 0:
			set_STATE(stand_left);
			break;
		case 1:
			set_STATE(stand_right);
			break;
		case 16:
			set_STATE(overbike_left);
			break;
		case 17:
			set_STATE(overbike_right);
			break;
		default:
			set_STATE(stand_right);
			break;
	}
}

void GameObject_Player::set_STATE(ObjectState state, bool final)
{
	on_climb = false;
	on_jump = false;

	switch(state)
	{
		case hammer_left:
			if((previous_STATE != hammer_left) && stick_time <=0)
				stick_time = PLAYER_STICK_TIME;
		case hammer_right:
			if((previous_STATE != hammer_right) && stick_time <=0)
				stick_time = PLAYER_STICK_TIME;
		case walk_right:
		case walk_left:
		case stand_right:
		case stand_left:
			if(STATE == duck_right ||
			   STATE == duck_left  ||
			   STATE == crawl_right ||
			   STATE == crawl_left)
			{
				//center stand sprite
				int delta_w = spr_object[STATE]->get_width() - spr_object[stand_left]->get_width();
				current_position->x += delta_w/2;
			}
			break;
		case shoot_right:
			if(previous_STATE != shoot_right)
			{
				int dir = 1; //right
				GameObject_Bullet *bullet = (GameObject_Bullet *) world->bullet;
				int bx = current_position->x + spr_object[state]->get_width();
				int by = current_position->y - floor(spr_object[state]->get_height()/2);

				bullet->deploy(bx, by, dir);
			}
			if(STATE == duck_right ||
			   STATE == duck_left  ||
			   STATE == crawl_right ||
			   STATE == crawl_left)
			{
				//center stand sprite
				int delta_w = spr_object[STATE]->get_width() - spr_object[stand_left]->get_width();
				current_position->x += delta_w/2;
			}
			break;

		case shoot_left:
			if(previous_STATE != shoot_left)
			{
				int dir = 0; //left
				GameObject_Bullet *bullet = (GameObject_Bullet *) world->bullet;
				int bx = current_position->x;
				int by = current_position->y - floor(spr_object[state]->get_height()/2);

				bullet->deploy(bx, by, dir);
			}
			if(STATE == duck_right ||
			   STATE == duck_left  ||
			   STATE == crawl_right ||
			   STATE == crawl_left)
			{
				//center stand sprite
				int delta_w = spr_object[STATE]->get_width() - spr_object[stand_left]->get_width();
				current_position->x += delta_w/2;
			}
			break;

		case climb:
		case climb_stop:
			on_climb = true;
			break;

		case jump_left:
		case jump_right:
			on_jump = true;
			if(STATE == duck_right ||
			   STATE == duck_left  ||
			   STATE == crawl_right ||
			   STATE == crawl_left)
			{
				//center jump sprite
				int delta_w = spr_object[STATE]->get_width() - spr_object[jump_left]->get_width();
				current_position->x += delta_w/2;
			}
			break;

		case dead:
			if(previous_STATE != dead)
			{
				anim_pos = 0;
				y_velocity = -PLAYER_JUMP_VELOCITY;
			}
			break;

		case duck_right:
		case crawl_right:
		case duck_left:
		case crawl_left:
			if(STATE != duck_right  &&
			   STATE != duck_left   &&
			   STATE != crawl_right &&
			   STATE != crawl_left )
			{
				//center crawl/duck sprite
				int delta_w = spr_object[duck_right]->get_width() - spr_object[STATE]->get_width();
				current_position->x -= delta_w/2;
			}
			break;
	}

	if(final)
		previous_STATE = STATE;
	STATE = state;
}

void GameObject_Player::update_collision_box()
{
	if(STATE == inactive)
		return;

	std::string action;

	switch(STATE)
	{
		case stand_left:
		case stand_right:
			action = "stand";
			break;
		case walk_right:
		case walk_left:
			action = "walk";
			break;
		case jump_right:
		case jump_left:
			action = "jump";
			break;
		case climb:
		case climb_stop:
			action = "climb";
			break;
		case crawl_right:
		case crawl_left:
			action = "crawl";
			break;
		case duck_right:
		case duck_left:
			action = "duck";
			break;
		case shoot_right:
		case shoot_left:
			action = "shoot";
			break;
		case hammer_right:
		case hammer_left:
			action = "hammer";
			break;
		case overbike_right:
		case overbike_left:
			action = "overbike";
			break;
		case dead:
			action = "dead";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/player/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Player::is_switch_trigger(RE_Rect* switch_button)
{
	if((STATE != hammer_left) && (STATE != hammer_right))
		return false; //only hammer STATE can switch a trigger button

	int hammer_x = current_position->x + CB.dx - 10; //left
	if(STATE == hammer_right)
	{
		hammer_x += CB.width + 10; //right
	}
	int hammer_y = current_position->y - CB.dy;


	//std::cout<< "switch LEFT="<<switch_button->Left()<<" TOP="<<switch_button->Top()<<" Right="<<switch_button->Right()<<" bottom:"<<switch_button->Bottom()<<"\n";
	//std::cout<< "LEFT="<<hammer_x<<" TOP="<<hammer_y - CB.height<<" Right="<<hammer_x + 10<<" bottom:"<<hammer_y<<"\n";
	RE_Rect* hammer_rect = new RE_Rect(hammer_x, hammer_y - CB.height, hammer_x + 10, hammer_y);

	bool ret = hammer_rect->is_overlapped(*switch_button);

	delete(hammer_rect);

	return ret;
}

bool GameObject_Player::is_stick(RE_Rect* object_box) //stick or punch stuns enemies
{
	if(((STATE != hammer_left) && (STATE != hammer_right)) || stick_time <= 0)
		return false; //only in hammer STATE can we stun an enemy

	int hammer_x = current_position->x + CB.dx - 10; //left
	if(STATE == hammer_right)
	{
		hammer_x += CB.width + 10; //right
	}
	int hammer_y = current_position->y - CB.dy;


	RE_Rect* hammer_rect = new RE_Rect(hammer_x, hammer_y - CB.height, hammer_x + 10, hammer_y);

	bool ret = hammer_rect->is_overlapped(*object_box);

	delete(hammer_rect);

	return ret;
}

bool GameObject_Player::turn(int time_elapsed)
{
	if(STATE == inactive)
		return false;

	if( STATE != dead &&
	   ( world->check_collision(kill_all, this) ||
	     world->check_collision(kill_player, this) ))
	{
		//std::cout << "I am dead at " << x << ","<< y<< "\n";
		set_STATE(dead);
	}

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(stick_time > 0)
		stick_time -= time_elapsed;

	bool ret = false;

	if(STATE != dead)
		ret = move(time_elapsed);
	else
		ret = move(time_elapsed, false);

	update_STATE();

	update_collision_box();

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	return ret;
}

void GameObject_Player::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	//get animation index
	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;


	if(STATE != dead)
	{
		//check for connection box collision
		int conn_index = world->map->check_connection_collision(get_collision_rect());
		if(conn_index >= 0) //we have a map connection
		{
			//change level
			sound->stop();
			world->change_level = true;
			world->connection_index = conn_index;
			return;
		}

		//check if we need to scroll down/up
		switch(scroll_mode)
		{
			case 2:
				world->next_scroll_y = current_position->y - world->gc->get_height()/2;
				break;

			default:
				if((current_position->y - view_y) >= (world->gc->get_height()-4*world->map->get_tile_height()))
					world->next_scroll_y += 8 * world->map->get_tile_height();
				else if((current_position->y - view_y - CB.height - 2*CB.dy) <= 4*world->map->get_tile_height())
					world->next_scroll_y -= 8 * world->map->get_tile_height();
				break;

		}

		//clip it
		if(world->next_scroll_y < 0)
			world->next_scroll_y = 0;
		if(world->next_scroll_y > world->map->get_map_height() - world->gc->get_height())
			world->next_scroll_y = world->map->get_map_height() - world->gc->get_height();

		//check if we need to scroll left/right
		if((current_position->x + CB.dx + CB.width - view_x) >  world->gc->get_width() - floor(world->map->get_tile_width()/2))
		{
			if((world->map->get_map_width() - view_x ) <= world->gc->get_width()) //last screen
			{
				//collide right
				if(STATE == walk_right ||
				   STATE == jump_right ||
				   STATE == crawl_right ||
				   STATE == overbike_right)
					current_position->x -= x_velocity; //moving right x_velocity is > 0
			}
			else
			{
				//scroll right
				world->next_scroll_x += world->gc->get_width();
				//move player completly to new screen
				current_position->x= world->next_scroll_x + 10;
			}
		}
		else if((current_position->x + CB.dx) - view_x < 0)
		{

			if(view_x < world->gc->get_width()) //last screen
			{
				//collide left
				if(STATE == walk_left ||
				   STATE == jump_left ||
				   STATE == crawl_left ||
				   STATE == overbike_left)
					current_position->x -= x_velocity; //moving left x_velocity is < 0
			}
			else
			{
				//scroll left
				world->next_scroll_x -= world->gc->get_width();
				//move player completly to new screen
				current_position->x= world->next_scroll_x + world->gc->get_width() - (CB.width  + CB.dx + 10);
			}
		}
	}

	play_sound();

	//coordinates start at 0 but since we use bottom position as sprite
	//reference the top position is y -(height-1)
	//example: for height=20 a sprite ref of (0,19) => top(0,0) : 19 -(20-1)
	// if we used sprite ref of (0,20) 20 would be in another (bottom) tile
	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() + 1) - view_y);
}
