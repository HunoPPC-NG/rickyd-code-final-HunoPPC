/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_moving.h"
#include "map.h"
/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving construction:

GameObject_Moving::GameObject_Moving(World *world)
: world(world), speed(1.0f)
{
	x_velocity = 0;
	y_velocity = 0;
	movement_speed = (double) GAME_FPS/1000;

	CB.stairs_dx = STAIRS_DX;
	CB.stairs_dy = 0;

	CB1.width = 0; //no CB1 by default
	CB2.width = 0; //no CB2 by default
	CB3.width = 0; //no CB3 by default

	CB_rect = NULL;
	CB1_rect = NULL;
	CB2_rect = NULL;
	CB3_rect = NULL;

	collision_left = false;
    collision_right = false;
    collision_top = false;
    collision_bottom = false;
    collision_stairs_bottom= false;
    collision_stairs = false;

    on_climb = false;
    on_jump = false;
}

//virtual
GameObject_Moving::~GameObject_Moving()
{
	if(CB_rect)
		delete(CB_rect);
	if(CB1_rect)
		delete(CB1_rect);
	if(CB2_rect)
		delete(CB2_rect);
	if(CB3_rect)
		delete(CB3_rect);
}

void GameObject_Moving::update_collision_box()
{
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving attributes:


/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving operations:
bool GameObject_Moving::can_walk()
{
	if(!x_velocity)
		return true;

	int try_x = current_position->x;
	int try_y = current_position->y + 1; //bottom tile

	if(x_velocity > 0)
		try_x += CB.width + CB.dx + (int) floor(x_velocity);
	else
		try_x -= (int) floor(-x_velocity);

	//check bottom tile
	switch(world->map->get_tile_type(try_x, try_y))
	{
		case Map::tile_top_stairs:
		case Map::tile_wall:
		case Map::tile_platform:
			return true;
			break;
	}

	return false;
}

bool GameObject_Moving::move(int time_elapsed, bool check_collision)
{

	dest_x = current_position->x;
	dest_y = current_position->y;

	if(x_velocity >= 0)
		dest_x += (int) floor(x_velocity);
	else
		dest_x -= (int) floor(-x_velocity);

	if(y_velocity >= 0)
	{
		if(y_velocity > MAX_Y_VELOCITY) //clip
			y_velocity = MAX_Y_VELOCITY;

		if(y_velocity > 1)
			dest_y += (int) floor(y_velocity /* * movement_speed * time_elapsed */);
	}
	else
	{
		if(y_velocity < -MAX_Y_VELOCITY) //clip
			y_velocity = -MAX_Y_VELOCITY;

		if(y_velocity < -1)
			dest_y -= (int) floor(-y_velocity /* * movement_speed * time_elapsed */);
	}
	//std::cout << "Move to:"<<dest_x<<","<<dest_y<<"\n";
	bool done = false;

	bool mvx = false;
	bool mvy = false;

	while(!done)
	{
		bool donex = false;
		bool doney = false;

		if(!donex)
		{
			if(dest_x > current_position->x)
				mvx = do_move_x(1, check_collision);
			else if(dest_x < current_position->x)
				mvx = do_move_x(-1, check_collision);
		}

		if(!doney)
		{
			if(dest_y > current_position->y)
				mvy = do_move_y(1, check_collision);
			else if(dest_y < current_position->y)
				mvy = do_move_y(-1, check_collision);
		}

		if(!mvx || current_position->x==dest_x)
			donex = true;
		if(!mvy || current_position->y==dest_y)
			doney = true;

		if(donex && doney)
			done = true;

		//check if are coliding with stairs left or right
		collision_stairs = false;
		for(int py = current_position->y - CB.dy - CB.height; py < current_position->y - CB.dy; py++)
		{
			//clip y to map height
			if(py >= world->map->get_map_height())
				break;
			//left
			int px = current_position->x + (CB.dx + CB.stairs_dx);
			//clip x to map width
			if(px >= world->map->get_map_width())
				break;

			if(world->map->get_tile_type(px, py) == Map::tile_stairs ||
			   world->map->get_tile_type(px, py) == Map::tile_top_stairs)
				collision_stairs = true;

			//right
			px = current_position->x + CB.dx + CB.width - CB.stairs_dx;
			//clip x to map width
			if(px >= world->map->get_map_width())
				break;
			if(world->map->get_tile_type(px, py) == Map::tile_stairs ||
			   world->map->get_tile_type(px, py) == Map::tile_top_stairs)
				collision_stairs = true;

		}

		collision_stairs_bottom = false;
		int py = current_position->y - CB.dy + 1;
		for(int px = current_position->x + (CB.dx + CB.stairs_dx); px < current_position->x + CB.dx + CB.width - CB.stairs_dx; px++)
		{
			//clip y to map height
			if(py >= world->map->get_map_height())
				break;
			//clip x to map width
			if(px >= world->map->get_map_width())
				break;

			if(world->map->get_tile_type(px, py) == Map::tile_stairs ||
			   world->map->get_tile_type(px, py) == Map::tile_top_stairs)
				collision_stairs_bottom = true;
			else if(world->map->get_tile_type(px, py) == Map::tile_wall ||
				    world->map->get_tile_type(px, py) == Map::tile_platform)
			{
				collision_stairs_bottom = false;
				break;
			}
		}
	}

	dest_x = current_position->x;
	dest_y = current_position->y;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving implementation:

bool GameObject_Moving::verify_move(int dx, int dy)
{
	if(!can_move)
		return false;

	int tix = current_position->x + CB.dx;
	int tfx = tix + CB.width - 1; // 0 indexed

	int tiy = current_position->y - CB.dy;
	int tfy = tiy - CB.height + 1; // 0 indexed

	int tx = tix + dx;
	int ty = tiy + dy;

	if (on_climb)
	{
		tix += CB.stairs_dx;
		tfx -= CB.stairs_dx;
	}

	RE_Point point;

	point.x = tx;
	point.y = ty;

	//---------- Horizontal movement
	if(dx > 0)
	{
		point.x += (CB.width -1);
	}

	if(point.x >=  world->map->get_map_width())
	{
		collision_right = true;
		return false;
	}

	if(point.x < 0)
	{
		collision_left = true;
		return false;
	}

	if(dx != 0)
	{
		collision_right = false;
		collision_left = false;

		for(point.y = ty; point.y > ty - CB.height; point.y--)
		{
			switch (world->map->get_tile_type(&point))
			{
				case Map::tile_wall:
					if(dx > 0)
						collision_right = true;
					else
						collision_left = true;
					break;
			}

			//no need for further checks
			if(collision_right || collision_left)
				return false;

			//check for collision with wall type objects
			if(world->check_point_collision(wall, &point) ||
			   world->check_point_collision(wall_elevator, &point, this))
			{
				//std::cout << "x collision wall object("<<point.x<<","<<point.y<<")\n";
				if(dx > 0)
					collision_right = true;
				else
					collision_left = true;
			}
/*
			//we are an elevator colliding with an object left/right
			if(get_type() == wall_elevator)
			{
				set_objects_iter(world->objects.begin());
				GameObject_Moving* col_obj;
				while(col_obj = (GameObject_Moving*) world->check_point_collision(any, &point, this, false))
				{
					if( col_obj->can_use_elevator() &&
					    col_obj->can_move)
					{
						//verify if move is possible
						if(!col_obj->do_move_x(dx))
							return false; //don't move
					}

					next_objects_iter();
				}
			}
*/
			if(collision_right || collision_left)
				return false;
		}

	}

	//---------- Vertical movement
	if(dy < 0)
	{
		point.y -= (CB.height -1);
	}

	if(point.y >=  world->map->get_map_height())
	{
		collision_bottom = true;
		return false;
	}

	if(point.y <  0)
	{
		collision_top = true;
		return false;
	}

	if(dy != 0)
	{
		collision_top = false;
		collision_bottom = false;

		for(point.x = tix+1; point.x < tfx; point.x++)
		{

			switch(world->map->get_tile_type(&point))
			{
				case Map::tile_wall:
					if(dy > 0 )
						collision_bottom = true;
					else
						collision_top = true;
					break;

				case Map::tile_platform:
					if (dy > 0)
					{
						if(!collision_stairs_bottom)
							collision_bottom = true;

						RE_Point point2;

						point2.y = point.y - 1;
						for(point2.x = tix; point2.x <= tfx; point2.x++)
						{
							if (world->map->get_tile_type(&point2) == Map::tile_platform)
							{
								collision_bottom = false; //inside platform
								break;
							}
						}
					}
					break;

				case Map::tile_top_stairs:
					if(dy > 0 && !on_climb)
					{
						collision_bottom = true;

						RE_Point point2;

						point2.y = point.y - 1;
						for(point2.x = tix; point2.x <= tfx; point2.x++)
						{
							if (world->map->get_tile_type(&point2) == Map::tile_top_stairs)
							{
								collision_bottom = false; //on stairs
								break;
							}
						}

					}
					break;
			}

			//no need for further checks
			if(collision_bottom || collision_top)
				return false;

			//check for collision with wall type objects
			if(world->check_point_collision(wall, &point) ||
			   world->check_point_collision(wall_elevator, &point, this))
			{
				//std::cout << "y collision wall object("<<point.x<<","<<point.y<<")\n";
				if(dy > 0 )
				{
					collision_bottom = true;
				}
				else
					collision_top = true;
			}
			else if(world->check_point_collision(platform_elevator, &point, this) ||
					world->check_point_collision(platform, &point, this))
			{
				if (dy > 0)
				{
					collision_bottom = true;

					RE_Point point2;
					point2.y = point.y - 1;
					for(point2.x = tix; point2.x <= tfx; point2.x++)
					{
						if (world->check_point_collision(platform_elevator, &point2, this) ||
						    world->check_point_collision(platform, &point2, this))
						{
							collision_bottom = false; //inside platform
							break;
						}
					}
				}
			}

/*
			//we are an elevator colliding with an object
			if(get_type() == wall_elevator)
			{
				GameObject_Moving* col_obj;
				set_objects_iter(world->objects.begin());
				while(col_obj = (GameObject_Moving*) world->check_point_collision(any, &point, this, false))
				{
					if( col_obj->can_use_elevator() &&
					    col_obj->can_move)
					{
						//verify if move is possible
						//if(!col_obj->do_move_y(dy))
						if(!col_obj->verify_move(0, dy))
							return false; //don't move
					}

					next_objects_iter();
				}
			}
*/
			if(collision_bottom || collision_top)
				return false;
		}

	}

	return true;
}

bool GameObject_Moving::do_move_x(int dx, bool check_collision)
{
	if(check_collision && verify_move(dx, 0) == false)
		return false;


	int tix = current_position->x + CB.dx;
	int tfx = tix + CB.width - 1; // 0 indexed

	int tiy = current_position->y - CB.dy;
	int tfy = tiy - CB.height + 1; // 0 indexed

	//we are an elevator colliding with object on top
	if(get_type() == wall_elevator)
	{
		RE_Rect* top_rect = new RE_Rect(tix, tfy - 2, tfx, tfy-1);

		//move elevator first
		current_position->x += dx;

		GameObject_Moving* col_obj;
		set_objects_iter(world->objects.begin());
		while(col_obj = (GameObject_Moving*) world->check_box_collision(any, top_rect, this, false))
		{
			if( col_obj->can_use_elevator() &&
				col_obj->can_move)
			{
				col_obj->do_move_x(dx); //try to move the object on top
				if(dx != 0)
					col_obj->on_elevator = true;
			}
			next_objects_iter();
		}
		delete(top_rect);
	}
	else if(get_type() == platform_elevator) //only move the object if all of it is on top
	{
		//check if object on top
		RE_Rect* top_rect = new RE_Rect(tix, tfy - 2, tfx, tfy-1);
		//check if object not below
		RE_Rect* bottom_rect = new RE_Rect(tix, tfy, tfx, tfy+2);

		//move elevator first
		current_position->x += dx;

		GameObject_Moving* col_obj;
		set_objects_iter(world->objects.begin());
		while(col_obj = (GameObject_Moving*) world->check_box_collision(any, top_rect, this, false))
		{
			if(!(col_obj->check_box_collision(bottom_rect)) &&
				col_obj->can_use_elevator() &&
				col_obj->can_move)
			{
				col_obj->do_move_x(dx); //try to move the object on top
				if(dx != 0)
					col_obj->on_elevator = true;
			}
			next_objects_iter();
		}
		delete(top_rect);
		delete(bottom_rect);
	}
	else
		current_position->x += dx;

	return true;
}

bool GameObject_Moving::do_move_y(int dy, bool check_collision)
{
	if (check_collision && verify_move(0, dy) == false)
 	{
 		y_velocity = 0;
		return false;
	}

	int tix = current_position->x + CB.dx;
	int tfx = tix + CB.width - 1; // 0 indexed

	int tiy = current_position->y - CB.dy;
	int tfy = tiy - CB.height + 1; // 0 indexed

	//we are an elevator colliding with object on top
	if(get_type() == wall_elevator)
	{
		RE_Rect* top_rect = new RE_Rect(tix, tfy - 2, tfx, tfy-1);

		//move elevator first
		current_position->y += dy;

		GameObject_Moving* col_obj;
		set_objects_iter(world->objects.begin());
		while(col_obj = (GameObject_Moving*) world->check_box_collision(any, top_rect, this, false))
		{
			if( col_obj->can_use_elevator() &&
				col_obj->can_move)
			{
				col_obj->do_move_y(dy); //try to move the object on top
				if(dy != 0)
					col_obj->on_elevator = true;
			}
			next_objects_iter();
		}
		delete(top_rect);
	}
	else if(get_type() == platform_elevator) //only move the object if all of it is on top
	{
		//check if object on top
		RE_Rect* top_rect = new RE_Rect(tix, tfy - 2, tfx, tfy-1);
		//check if object not below
		RE_Rect* bottom_rect = new RE_Rect(tix, tfy, tfx, tfy+2);

		//move elevator first
		current_position->y += dy;

		GameObject_Moving* col_obj;
		set_objects_iter(world->objects.begin());
		while((col_obj = (GameObject_Moving*) world->check_box_collision(any, top_rect, this, false)) != NULL)
		{
			if(!(col_obj->check_box_collision(bottom_rect)) &&
				col_obj->can_use_elevator() &&
				col_obj->can_move)
			{
				col_obj->do_move_y(dy); //try to move the object on top
				if(dy != 0)
					col_obj->on_elevator = true;
			}
			next_objects_iter();
		}

		delete(top_rect);
		delete(bottom_rect);
	}
	else
		current_position->y += dy;

	return true;
}

bool GameObject_Moving::can_stand(CollisionBox &stand_box)
{
	RE_Point point;
	point.y = current_position->y - stand_box.dy - stand_box.height;

	for(point.x = current_position->x + stand_box.dx; point.x < current_position->x + stand_box.dx + stand_box.width; point.x++)
	{
		if (world->map->get_tile_type(&point) == Map::tile_wall ||
			world->check_point_collision(wall, &point))
			return false;
	}

	return true;
}

RE_Rect* GameObject_Moving::get_collision_rect()
{
	//coordinates start at 0 but since we use bottom position as sprite
	//reference the top position is y -(height-1)
	//example: for height=20 a sprite ref of (0,19) => top(0,0) : 19 -(20-1)
	// if we used sprite ref of (0,20) 20 would be in another (bottom) tile

	int left=0;
	int top=0;
	int right=0;
	int bottom=0;

	if(CB1.width > 0) //dx,dy = deltas to current_position
	{
		left = current_position->x + CB1.dx;
		top = current_position->y + CB1.dy - CB1.height - 1;
		right = current_position->x + CB1.dx + CB1.width;
		bottom = current_position->y + CB1.dy;

		if(!CB1_rect)
			CB1_rect = new RE_Rect(left, top, right, bottom);
		else
		{
			CB1_rect->Left(left);
			CB1_rect->Top(top);
			CB1_rect->Right(right);
			CB1_rect->Bottom(bottom);
		}
	}

	if(CB2.width > 0) //dx,dy = deltas to current_position
	{
		left = current_position->x + CB2.dx;
		top = current_position->y + CB2.dy - CB2.height - 1;
		right = current_position->x + CB2.dx + CB2.width;
		bottom = current_position->y + CB2.dy;

		if(!CB2_rect)
			CB2_rect = new RE_Rect(left, top, right, bottom);
		else
		{
			CB2_rect->Left(left);
			CB2_rect->Top(top);
			CB2_rect->Right(right);
			CB2_rect->Bottom(bottom);
		}
	}

	if(CB3.width > 0) //dx,dy = deltas to current_position
	{
		left = current_position->x + CB3.dx;
		top = current_position->y + CB3.dy - CB3.height - 1;
		right = current_position->x + CB3.dx + CB3.width;
		bottom = current_position->y + CB3.dy;

		if(!CB3_rect)
			CB3_rect = new RE_Rect(left, top, right, bottom);
		else
		{
			CB3_rect->Left(left);
			CB3_rect->Top(top);
			CB3_rect->Right(right);
			CB3_rect->Bottom(bottom);
		}
	}

	if(CB2.width > 0 || CB1.width > 0 || CB3.width > 0) //dx,dy = deltas to current_position for CB
	{
		left = current_position->x + CB.dx;
		top = current_position->y + CB.dy - CB.height - 1;
		right = current_position->x + CB.dx + CB.width;
		bottom = current_position->y + CB.dy;
	}
	else //dx,dy = deltas for width, height for CB if no CB1, CB2 present
	{
		left = current_position->x + CB.dx;
		top = current_position->y - (CB.dy + CB.height - 1);
		right = current_position->x + (CB.dx + CB.width);
		bottom = current_position->y - CB.dy;
	}

	if(!CB_rect)
		CB_rect = new RE_Rect(left, top, right, bottom);
	else
	{
		CB_rect->Left(left);
		CB_rect->Top(top);
		CB_rect->Right(right);
		CB_rect->Bottom(bottom);
	}

	return CB_rect;
}

bool GameObject_Moving::check_box_collision(RE_Rect *box)
{
	if(get_collision_rect()->is_overlapped(box))
		return true;

	if(CB1.width > 0 && CB1_rect && CB1_rect->is_overlapped(box))
		return true;

	if(CB2.width > 0 && CB2_rect && CB2_rect->is_overlapped(box))
		return true;

	if(CB3.width > 0 && CB3_rect && CB3_rect->is_overlapped(box))
		return true;

	return false;
}

bool GameObject_Moving::check_point_collision(RE_Point *point)
{
	if(point->is_inside(get_collision_rect()))
		return true;

	return false;
}
