/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_bullet.h"
#include "tinyxml2.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Bullet construction:

GameObject_Bullet::GameObject_Bullet(World *world) :
	GameObject_Moving(world)
{
	set_type(kill_all);
	set_use_elevator(false);

	spr_object[overbike_right] = NULL;
	spr_object[overbike_left] = NULL;

	spr_object[moving_right] = new RE_Sprite(world->gc, "global/objects/bullet/move/right/sprite", world->global_resources);
	spr_object[moving_left]  = new RE_Sprite(world->gc, "global/objects/bullet/move/left/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/bullet/overbike/right/sprite"))
		spr_object[overbike_right] = new RE_Sprite(world->gc, "global/objects/bullet/overbike/right/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/bullet/overbike/left/sprite"))
		spr_object[overbike_left] = new RE_Sprite(world->gc, "global/objects/bullet/overbike/left/sprite", world->global_resources);

	sfx_shoot = new RE_SoundBuffer("global/objects/bullet/sounds/shoot/sample", world->global_resources);
	sfx_shoot->set_volume(120);

	sound = new RE_SoundSession();

	set_STATE(inactive);

    set_total(MAX_BULLETS);

	anim_pos = 0;

	i_am_dead = false;
	set_object_collision(NULL);

	current_position->x = 0;
	current_position->y = 0;

	turn_time = 0;
	trigger_time = 0;
}

GameObject_Bullet::~GameObject_Bullet()
{
	delete(spr_object[moving_right]);
	delete(spr_object[moving_left]);

	if(spr_object[overbike_right])
		delete(spr_object[overbike_right]);

	if(spr_object[overbike_left])
		delete(spr_object[overbike_left]);

	delete (sound);
	delete (sfx_shoot);
}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Bullet attributes:

/////////////////////////////////////////////////////////////////////////////
int GameObject_Bullet::get_dir_width(int dir)
{
	if(dir < inactive && spr_object[dir] != NULL)
		return spr_object[dir]->get_width();
	else
		return 0;
}

int GameObject_Bullet::get_dir_height(int dir)
{
	if(dir < inactive && spr_object[dir] != NULL)
		return spr_object[dir]->get_height();
	else
		return 0;
}

bool GameObject_Bullet::in_window(ObjectState state)
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

void GameObject_Bullet::update_STATE()
{
	//check for hit
	GameObject* obj = get_object_collision();
	if(collision_left || collision_right || obj != NULL)
	{
		//if(obj)
		//	std::cout << "bullet collided with object id:"<<obj->get_id()<<"\n";
		//else
		//	std::cout << "bullet collided with wall letf:"<<collision_left<<" right:"<<collision_right<<"\n";

		set_STATE(inactive);
	}

	if(current_position->x < world->scroll_x ||
	   current_position->x > world->scroll_x + world->gc->get_width())
		set_STATE(inactive);
}

//shoot a bullet
void GameObject_Bullet::deploy(int start_x, int start_y, int dir)
{
	if( STATE != inactive ||
	    bullets_available <= 0 ||
	    trigger_time > 0)
		return;

	//reset prior collisions
	set_object_collision(NULL);
	collision_left = false;
	collision_right = false;

	current_position->x = start_x;
	current_position->y = start_y;

	switch(dir)
	{
		// take bullet velocity into account as move (bullet turn)
		// it's called right after deployment by player
		case 3:
			set_STATE(overbike_right);
			current_position->x -= BULLET_VELOCITY;
			break;
		case 2:
			set_STATE(overbike_left);
			current_position->x -= get_dir_width(3) - BULLET_VELOCITY;
			break;
		case 1:
			set_STATE(moving_right);
			current_position->x -= BULLET_VELOCITY;
			break;
		default:
		case 0:
			set_STATE(moving_left);
			current_position->x -= get_dir_width(0) - BULLET_VELOCITY;;
			break;
	}

	sound->play(sfx_shoot);
}

void GameObject_Bullet::set_STATE(ObjectState state)
{
	switch(state)
	{
		case inactive:
			set_type(kill_none);
			break;

		case overbike_right:
			if(spr_object[state] == NULL)
			{
				state = inactive;
				set_type(kill_none);
				break;
			}
			bullets_available--;
			trigger_time = BULLET_TRIGGER_TIME;
			set_type(kill_all);
			x_velocity = BULLET_VELOCITY;
			break;

		case overbike_left:
			if(spr_object[state] == NULL)
			{
				state = inactive;
				set_type(kill_none);
				break;
			}
			bullets_available--;
			trigger_time = BULLET_TRIGGER_TIME;
			set_type(kill_all);
			x_velocity = -BULLET_VELOCITY;
			break;

		case moving_right:
			bullets_available--;
			trigger_time = BULLET_TRIGGER_TIME;
			set_type(kill_all);
			x_velocity = BULLET_VELOCITY;
			break;

		default:
		case moving_left:
			bullets_available--;
			trigger_time = BULLET_TRIGGER_TIME;
			set_type(kill_all);
			x_velocity = -BULLET_VELOCITY;
			break;
	}

	if(bullets_available < 0) //clip
		bullets_available = 0;

	previous_STATE = STATE;
	STATE = state;

	update_collision_box();
}


int GameObject_Bullet::get_total()
{
	return bullets_available;
}

void GameObject_Bullet::set_total(int bullets)
{
	bullets_available = bullets;
	if(bullets_available > MAX_BULLETS)
		bullets_available = MAX_BULLETS;
}

void GameObject_Bullet::update_collision_box()
{
	std::string action;

	switch(STATE)
	{
		case overbike_right:
		case overbike_left:
			action = "overbike";
			break;
		case moving_left:
		case moving_right:
		default:
			action = "move";
			break;
	}

	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/bullet/"+action+"/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Bullet::turn(int time_elapsed)
{
	trigger_time -= time_elapsed;
	if(trigger_time < 0)
		trigger_time = 0;

	if(STATE == inactive)
		return false;

	y_velocity = 0;

	if(turn_time < speed)
		turn_time += time_elapsed;
	else
		turn_time = 0;

	if(turn_time == 0) //rotate on turn start
		anim_pos++;

	bool ret = move(time_elapsed);

	update_STATE();

	return ret;
}

void GameObject_Bullet::show(int view_x, int view_y)
{
	if(STATE == inactive)
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	//only render if in window
	if(!in_window(STATE))
		return;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}

