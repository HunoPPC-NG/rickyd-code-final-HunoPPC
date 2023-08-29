/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_roller.h"
#include "re_sprite.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Roller construction:

GameObject_Roller::GameObject_Roller(World *world, tinyxml2::XMLElement* object_element, const std::string object_name) :
	GameObject_Moving(world)
{
	obj_name = object_name;

	set_type(kill_none);

	spr_object[moving_left] = NULL;
	spr_object[moving_right] = NULL;
	spr_object[waiting] = NULL;
	spr_object[delayed] = NULL;
	sfx_moving = NULL;

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/moving/left/sprite"))
		spr_object[moving_left] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/moving/left/sprite", world->global_resources);

	if(world->global_resources->file_exists("global/objects/"+obj_name+"/moving/right/sprite"))
		spr_object[moving_right] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/moving/right/sprite", world->global_resources);


	initial_state = 0;
	initial_x = object_element->FirstChildElement("position")->IntAttribute("x");
	initial_y = object_element->FirstChildElement("position")->IntAttribute("y");
	current_position->x = initial_x;
	current_position->y = initial_y;
	object_element->FirstChildElement("position")->QueryIntAttribute("state", &initial_state);

	if(initial_state > 1 || initial_state < -1)
	{
		if(world->global_resources->file_exists("global/objects/"+obj_name+"/waiting/right/sprite"))
			spr_object[waiting] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/waiting/right/sprite", world->global_resources);

		if(world->global_resources->file_exists("global/objects/"+obj_name+"/delayed/right/sprite"))
			spr_object[delayed] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/delayed/right/sprite", world->global_resources);
	}
	else
	{
		if(world->global_resources->file_exists("global/objects/"+obj_name+"/waiting/left/sprite"))
			spr_object[waiting] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/waiting/left/sprite", world->global_resources);

		if(world->global_resources->file_exists("global/objects/"+obj_name+"/delayed/left/sprite"))
			spr_object[delayed] = new RE_Sprite(world->gc, "global/objects/"+obj_name+"/delayed/left/sprite", world->global_resources);
	}


	if(world->global_resources->file_exists("global/objects/"+obj_name+"/sounds/moving/sample"))
	{
		sfx_moving = new RE_SoundBuffer("global/objects/"+obj_name+"/sounds/moving/sample", world->global_resources);
		sfx_moving->set_volume(1.0);
	}

	sound = new RE_SoundSession();

	if(initial_state > 0 && !spr_object[moving_right])
	{
		std::cerr<< "GameObject_Roller: (FATAL) direction is set to right but no matching sprite found\n";
		exit(-1);
	}
	if(initial_state == 0 && !spr_object[moving_left])
	{
		std::cerr<< "GameObject_Roller: (FATAL) direction is set to left but no matching sprite found\n";
		exit(-1);
	}

	TriggerBox = NULL;

	//check for trigger box
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

	BoundingBox = NULL;

	//check for bounding box
	tinyxml2::XMLElement* bounding_element = NULL;
	bounding_element = object_element->FirstChildElement("bounding_box");
	if(bounding_element)
	{
		int bb_x = bounding_element->IntAttribute("x");
		int bb_y = bounding_element->IntAttribute("y");
		int bb_width = bounding_element->IntAttribute("width");
		int bb_height = bounding_element->IntAttribute("height");

		set_bounding_box(bb_x, bb_y, bb_width, bb_height);
	}

	bounce = false;
	bullet_trigger = true;
	bomb_trigger = true;
	max_falls = 1;
	try_change = 0;
	def_velocity = ROLLER_VELOCITY;
	def_trigger_delay = 0;
	def_trigger_time  = 0;
	int anim_velocity = 4;
	//check for parameters
	tinyxml2::XMLElement* param_element = NULL;
	param_element = object_element->FirstChildElement("parameters");
	if(param_element)
	{
		param_element->QueryIntAttribute("velocity", &def_velocity);
		param_element->QueryIntAttribute("anim_velocity", &anim_velocity);
		param_element->QueryIntAttribute("trigger_time", &def_trigger_time);
		param_element->QueryIntAttribute("trigger_delay", &def_trigger_delay);
		param_element->QueryBoolAttribute("bounce", &bounce);
		param_element->QueryBoolAttribute("bullet_trigger", &bullet_trigger);
		param_element->QueryBoolAttribute("bomb_trigger", &bomb_trigger);
		param_element->QueryIntAttribute("max_falls", &max_falls);
		param_element->QueryIntAttribute("try_change", &try_change);
	}

	falls = 0;
	set_STATE(waiting);
	update_collision_box();

	anim_pos  = 0;
	turn_time = 0;
	move_time = 0;
	trigger_time = 0;
	trigger_delay = def_trigger_delay;
	delay_ind = 0;
	falling = false;

	set_speed(anim_velocity); //animation speed in frames per sec
}

GameObject_Roller::~GameObject_Roller()
{
	if(spr_object[moving_right])
		delete (spr_object[moving_right]);
	if(spr_object[moving_left])
		delete (spr_object[moving_left]);
	if(spr_object[waiting])
		delete (spr_object[waiting]);
	if(TriggerBox)
		delete(TriggerBox);
	if(BoundingBox)
		delete(BoundingBox);
	if(sfx_moving)
		delete(sfx_moving);

	delete(sound);

}
/////////////////////////////////////////////////////////////////////////////
// GameObject_Roller attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Roller operations:

void GameObject_Roller::set_trigger_box(int tb_x, int tb_y, int tb_width, int tb_height)
{
	TriggerBox = new RE_Rect(tb_x, tb_y - tb_height, tb_x + tb_width, tb_y);
}

void GameObject_Roller::set_bounding_box(int bb_x, int bb_y, int bb_width, int bb_height)
{
	BoundingBox = new RE_Rect(bb_x, bb_y - bb_height, bb_x + bb_width, bb_y);
}

bool GameObject_Roller::check_trigger()
{
	if( TriggerBox != NULL &&
		world->player->get_type() != kill_none)
	{
		if (bullet_trigger && world->bullet->get_type() == kill_all &&
	         check_box_collision(world->bullet->get_collision_rect()))
		{	//bullet collision
			world->bullet->set_object_collision(this);
			return true;
		}
		else if (bomb_trigger && world->bomb->get_type() == kill_all &&
			 check_box_collision(world->bomb->get_collision_rect()))
		{	//bomb collision
			world->bomb->set_object_collision(this);
			return true;
		}

		//check player collision
		if(world->player->get_type() != kill_none)
			return(TriggerBox->is_overlapped(world->player->get_collision_rect()));
	}
	else if (!TriggerBox)
		return true;

	return false;
}

bool GameObject_Roller::in_window(ObjectState state)
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

void GameObject_Roller::update_STATE()
{
	if(STATE == inactive)
		return;

	if(STATE == waiting && trigger_time <= 0)
	{
		if(check_trigger())
		{
			if(!delay_ind)
				trigger_delay = def_trigger_delay;
			else
				trigger_delay = 0; //no more delay

			trigger_time = def_trigger_time;

			set_STATE(delayed);
		}
		else
		{
			delay_ind = 0; //not triggered restart delay
		}

	}

	if(STATE == delayed && trigger_delay <= 0)
	{
		delay_ind = -1;//no more delay while triggered

		set_state(initial_state);
	}

	if(trigger_delay > 0)
	{
		trigger_delay -= turn_time;
	}

	if(trigger_time > 0)
	{
		trigger_time -= turn_time;
	}

	if(STATE == waiting ||
	   STATE == delayed)
		return;

	if(in_window(STATE) && !sound->is_playing())
		sound->play(sfx_moving);

	//we are already falling
	if(falling)
	{
		if(falls > 1)
			x_velocity = 0;

		if(BoundingBox)
			if(current_position->y >= BoundingBox->Bottom())
			{
				if(initial_state < 0)
					set_STATE(waiting); //repeat
				else
					set_STATE(inactive); //don't repeat
			}
	}
	else if(verify_move(0,1)) //if we can go down we are falling
	{
		falling = true;

		if(falls > 1)//if not the first fall
			x_velocity = 0;
		else //if it's the first fall
		{
			if(STATE == moving_right)
				x_velocity = def_velocity;
			if(STATE == moving_left)
				x_velocity = -def_velocity;
		}
		falls++;
	}

	if(collision_bottom) //not falling
	{
		if(falls >= max_falls) //max falls reached
		{
			if(initial_state < 0)
				set_STATE(waiting); //repeat
			else
				set_STATE(inactive); //don't repeat
		}

		if(falling && falls > try_change)
		{
			switch(STATE)
			{
				case moving_right:
					//verify left half a tile
					if(verify_move(-10,0))
					{
						set_STATE(moving_left);
						collision_left = false;
					}
					break;
				case moving_left:
					//verify right half a tile
					if(verify_move(10,0))
					{
						set_STATE(moving_right);
						collision_right = false;
					}
					break;
			}
		}

		falling = false;

		bool boundbox_collide = false;
		if(BoundingBox)
		   if(current_position->x >= BoundingBox->Right() ||
		      current_position->x <= BoundingBox->Left())
				boundbox_collide = true;

		if((STATE == moving_right && collision_right) ||
		   (STATE == moving_left  && collision_left) ||
		    boundbox_collide)
		{

			if(bounce) //invert direction
			{
				if(STATE == moving_right)
					set_STATE(moving_left);
				else
					set_STATE(moving_right);
			}
			else
			{
				if(initial_state < 0)
					set_STATE(waiting); //repeat
				else
					set_STATE(inactive); //don't repeat
			}
		}

		if(STATE == moving_right)
			x_velocity = def_velocity;
		if(STATE == moving_left)
			x_velocity = -def_velocity;
	}

	//gravity
	y_velocity += GRAVITY;
}

void GameObject_Roller::set_state(int state)
{
	switch(state)
	{
		case -2:
		case 2:
			set_STATE(moving_right);
			break;
		case -1:
		case 1:
			set_STATE(moving_left);
			break;
		default:
			set_STATE(moving_left);
			initial_state=1;//no repeat
			break;
	}
}

void GameObject_Roller::set_STATE(ObjectState state)
{
	if(state == inactive || state == waiting)
	{
		set_type(kill_none);
		falling = false;
		if(sound->is_playing())
			sound->stop();
		falls = 0;
		current_position->x = initial_x;
		current_position->y = initial_y;
		collision_right = false;
		collision_left = false;
	}

	if(state == moving_right || state == moving_left)
		set_type(kill_all);

	//std::cout << "Roller("<<get_id()<<"): STATE "<< STATE <<"->"<<state<<"\n";
	previous_STATE = STATE;
	STATE = state;
}


void GameObject_Roller::update_collision_box()
{
	tinyxml2::XMLElement* cb_element = world->global_resources->getElement("global/objects/"+obj_name+"/moving/collision_box");
	CB.width = cb_element->IntAttribute("width");
    CB.height = cb_element->IntAttribute("height");
    CB.dx = cb_element->IntAttribute("dx");
    CB.dy = cb_element->IntAttribute("dy");
}

bool GameObject_Roller::turn(int time_elapsed)
{
	turn_time = time_elapsed;

	update_STATE();

	if(	STATE == waiting ||
		STATE == delayed ||
		STATE == inactive)
		return false;

	if(move_time < speed)
		move_time += time_elapsed;
	else
		move_time = 0;

	if(move_time == 0) //update animation
		anim_pos++;

	bool ret = false;
	ret = move(time_elapsed);

	return ret;
}

void GameObject_Roller::show(int view_x, int view_y)
{
	if( STATE == inactive )
		return;

	//only render if in window (checks for STATE sprite)
	if(!in_window(STATE))
		return;

	int anim_length = spr_object[STATE]->get_frame_count();
	if (anim_pos > anim_length-1) anim_pos=0;

	spr_object[STATE]->set_frame(anim_pos);
	spr_object[STATE]->draw(
		current_position->x  - view_x,
		(current_position->y - spr_object[STATE]->get_height() +1) - view_y);
}
