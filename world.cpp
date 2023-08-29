/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include <stdlib.h>
#include "precomp.h"
#include "tinyxml2.h"
#include "world.h"
#include "gameobject_factory.h"

/////////////////////////////////////////////////////////////////////////////
// World construction:

World::World(RE_GameContext* pgc, RE_ResourceManager* resource, int start_map, int start_submap)
{
	gc = pgc;
	data_dir = resource->get_data_dir();

	scores = new Scores();

	Levels = new RE_LevelManager("levels.xml", data_dir);

	game_resources = resource;

	score = 0;

	c4a = 0;

	menu_font  = new RE_Font(gc, "global/menu/font", game_resources);
	paused_font  = new RE_Font(gc, "global/menu/font", game_resources);
	paused_font->set_text("Paused");

	gameover_font  = new RE_Font(gc, "global/gameover/font", game_resources);
	gameover_font->set_text("Game Over");
	sfx_gameover = new RE_SoundBuffer("global/gameover/music/sample", game_resources);
	sfx_gameover->set_volume(1.0f);

	sfx_submap = NULL; //load from global map file

	sound = new RE_SoundSession();

    scroll_x = 0;
    scroll_y = 0;
    next_scroll_x = 0;
    next_scroll_y = 0;

    change_level = false;

    lifes = MAX_LIFES;
    game_over = false;
	intro_shown = false;
	welcome_shown = false;
	process_score = false;
	process_keys = false;

	map_num = start_map;
	submap_num = start_submap;
	location_index = 0;

	curr_map = map_num;
	curr_submap = submap_num;
	curr_location = location_index;

//	  std::cout << "loading global object resources\n";
	global_resources = new RE_ResourceManager(Levels->get_global_file(map_num), false, data_dir);

	spr_score_back = new RE_Sprite(gc, "global/objects/score/background/sprite", global_resources);
	spr_score_bombs = new RE_Sprite(gc, "global/objects/score/bombs/sprite", global_resources);
	spr_score_bullets = new RE_Sprite(gc, "global/objects/score/bullets/sprite", global_resources);
	spr_score_lifes = new RE_Sprite(gc, "global/objects/score/lifes/sprite", global_resources);
	score_font = new RE_Font(gc, "global/objects/score/font", global_resources);

	map_resources = NULL;

	events = new RE_Events();

	keys_dialog = NULL;
	welcome_dialog = NULL;
	score_dialog = NULL;
	welcome = NULL;
	intro = NULL;
	map = NULL;
	player = NULL;
	bomb = NULL;
	bullet = NULL;

}

World::~World()
{
	for (
		std::list<GameObject*>::iterator it = objects.begin();
		it != objects.end();
		it++)
	{
		delete *it;
	}
	objects.clear();

	bonus_list.clear();

	delete(map);

	if(intro)
		delete(intro);
	delete(spr_score_back);
	delete(spr_score_bombs);
	delete(spr_score_bullets);
	delete(spr_score_lifes);
	if(score_dialog)
		delete(score_dialog);
	if(welcome_dialog)
		delete(welcome_dialog);
	if(keys_dialog)
		delete(keys_dialog);
	if(sfx_submap)
		delete(sfx_submap);
	delete(sfx_gameover);
	delete(sound);
	delete(score_font);
	delete(menu_font);
	delete(paused_font);
	delete(gameover_font);
	delete(events);
	delete(global_resources);
	delete(map_resources);
	delete(scores);
}

/////////////////////////////////////////////////////////////////////////////
// World attributes:

/////////////////////////////////////////////////////////////////////////////
// World operations:

void World::reset_level(bool global_change)
{
//	  std::cout << "reseting to level "<< map_num<<" - "<< submap_num<<"\n";

	int n_bullets = 6;
	if(bullet != NULL)
		n_bullets= bullet->get_total();
	int n_bombs = 6;
	if(bomb != NULL)
		n_bombs = bomb->get_total();

	if(map_num < 1 || submap_num < 1)
	{
		std::cerr << "Invalid Level reference "<< map_num<<" - "<< submap_num<<"\n";
		game_over = true;
		//update score
		score += lifes * 300; //lifes bonus
		score += n_bombs * 10;   //bombs bonus
		score += n_bullets * 10; //bullets bonus

		return;
	}

	if(map_num > curr_map)
	{
		//unlock next level
		Levels->map_unlock(map_num);

		curr_map = map_num;
		curr_submap = submap_num; //reset submap
		curr_location = location_index;

		intro_shown = false;
		global_change = true;

		score += lifes * 300; //lifes bonus
		score += n_bombs * 10;   //bombs bonus
		score += n_bullets * 10; //bullets bonus

	}

	if(submap_num > curr_submap)
	{
		sound->play(sfx_submap);

		curr_submap = submap_num;
		curr_location = location_index;
		score += lifes * 300;    //lifes bonus
		score += n_bombs * 10;   //bombs bonus
		score += n_bullets * 10; //bullets bonus
	}

	for (
		std::list<GameObject*>::iterator it = objects.begin();
		it != objects.end();
		it++)
	{
		if(*it != NULL)
			delete *it;
	}
	objects.clear();

	if(global_change)
	{
		//load global object resources
		if(global_resources)
			delete(global_resources);
		global_resources = new RE_ResourceManager(Levels->get_global_file(map_num), false, data_dir);

		//reload score tab
		delete(spr_score_back);
		delete(spr_score_bombs);
		delete(spr_score_bullets);
		delete(spr_score_lifes);
		delete(score_font);
		spr_score_back = new RE_Sprite(gc, "global/objects/score/background/sprite", global_resources);
		spr_score_bombs = new RE_Sprite(gc, "global/objects/score/bombs/sprite", global_resources);
		spr_score_bullets = new RE_Sprite(gc, "global/objects/score/bullets/sprite", global_resources);
		spr_score_lifes = new RE_Sprite(gc, "global/objects/score/lifes/sprite", global_resources);
		score_font = new RE_Font(gc, "global/objects/score/font", global_resources);

		//reload sfx
		if(sfx_submap)
			delete(sfx_submap);
		sfx_submap = NULL;
		if(global_resources->file_exists("global/submap/music/sample"))
		{
			sfx_submap = new RE_SoundBuffer("global/submap/music/sample", global_resources);
			sfx_submap->set_volume(1.0f);
		}
	}

	if(map_resources)
    	delete(map_resources);
    map_resources = new RE_ResourceManager(Levels->get_file(map_num, submap_num), false, data_dir);

	if(map)
		delete map;
	map = new Map(map_resources, gc);

	scroll_x = map->get_scroll_x(location_index);
    scroll_y = map->get_scroll_y(location_index);
	next_scroll_x = scroll_x;
	next_scroll_y = scroll_y;

	//Add map objects
	tinyxml2::XMLElement* objects_element = map_resources->getElement("map/submap/objects");
	tinyxml2::XMLElement* object_element = NULL;
	for(object_element = objects_element->FirstChildElement("object"); object_element != NULL; object_element = object_element->NextSiblingElement("object"))
	{
		std::string objectName = object_element->Attribute("class");
		GameObject* g_obj = GameObjectFactory::Get()->CreateGameObject(objectName, this, object_element);

		if(g_obj != NULL)
			objects.push_back(g_obj);
	}

	// Add player object
	player = GameObjectFactory::Get()->CreateGameObject("Player", this, NULL);
	objects.push_back(player);

	//Add bullet object
	bullet = GameObjectFactory::Get()->CreateGameObject("Bullet", this, NULL);
	objects.push_back(bullet);

	//Add bomb object
	bomb = GameObjectFactory::Get()->CreateGameObject("Bomb", this, NULL);
	objects.push_back(bomb);

	bullet->set_total(n_bullets);
	bomb->set_total(n_bombs);
}


void World::add_score(int bonus_score)
{
	score += bonus_score;
}

void World::add_bonus(Uint32 id)
{
	//check if bonus already exists
	for (
		std::list<Bonus>::iterator iter = bonus_list.begin();
		iter != bonus_list.end();
		iter++)
	{
		if((*iter).id == id &&
			(*iter).map == map_num &&
			(*iter).submap == submap_num)
			return; //already in the list
	}

	//add the bonus
	Bonus my_bonus;
	my_bonus.id = id;
	my_bonus.map = map_num;
	my_bonus.submap = submap_num;
	my_bonus.active = true;

	bonus_list.push_back(my_bonus);

	//std::cout << "adding bonus "<<map_num<<"-"<<submap_num<<"-"<< id <<" to list\n";
}

void World::disable_bonus(Uint32 id)
{
	//check for bonus
	for (
		std::list<Bonus>::iterator iter = bonus_list.begin();
		iter != bonus_list.end();
		iter++)
	{
		if((*iter).id == id &&
			(*iter).map == map_num &&
			(*iter).submap == submap_num)
		{
			(*iter).active = false; //disable it
			return; //don't look any further
		}
	}
}

bool World::check_bonus(Uint32 id)
{
	//check for bonus
	for (
		std::list<Bonus>::iterator iter = bonus_list.begin();
		iter != bonus_list.end();
		iter++)
	{
		if((*iter).id == id &&
			(*iter).map == map_num &&
			(*iter).submap == submap_num)
		{
			return (*iter).active; //return the bonus state
		}
	}

	return false; //false if bonus not in list
}

void World::run()
{
	reset_level(true);

	RE_Point mouse_pointer;
	quit = false;
	bool paused = false;
	bool pause_key_up = false;

	Uint32 current_time = SDL_GetTicks();
	Uint32 start_time = current_time;
	Uint64 frame_counter = 0;
	Uint32 loop_time = start_time; //loop time (without time_wait)
	Uint32 time_wait = 0;
	Uint32 time_elapsed = 0; //total elapsed time (with time_wait)
	Uint32 turn_time = 1000/GAME_FPS; //turn speed

	while (!quit)
	{
		frame_counter++;

		time_elapsed = current_time;
		current_time = SDL_GetTicks();
		time_elapsed = current_time - time_elapsed;

		loop_time = current_time - loop_time;

		if(loop_time >= turn_time)
			time_wait = 0;
		else
			time_wait = turn_time-loop_time; //run game at aprox GAME_FPS

		//std::cout << "tl:"<< loop_time << "tw:" << time_wait << "te:" << time_elapsed << "\n";

		if(time_wait > 0)
			SDL_Delay(time_wait);

		loop_time = SDL_GetTicks();

		//map->set_scroll(scroll_x, scroll_y);
		gc->clear(RE_GameContext::Color_Black);

		if(process_keys)
		{
			//keys menu
			if(!keys_dialog)
				keys_dialog = new KeysDialog(gc, global_resources, events);

			keys_dialog->update(time_elapsed);
			keys_dialog->show(time_elapsed);

			if(keys_dialog->is_commited())
			{
				process_keys = false;
				delete(keys_dialog);
				keys_dialog = NULL;
			}

		}
		else if(!welcome_shown)
		{
			//welcome screen and menu
			if(!welcome)
				welcome = new Welcome(gc, game_resources, scores);

			if(!welcome_dialog)
				welcome_dialog = new WelcomeDialog(gc, global_resources, events, Levels, map_num);

			welcome_dialog->update(time_elapsed);

			welcome->show(time_elapsed);
			welcome_dialog->show(time_elapsed);

			if(welcome_dialog->is_commited())
			{
				switch(welcome_dialog->get_response())
				{
					case (WelcomeDialog::PLAY): // we choose a map to play
					{
						map_num = welcome_dialog->get_selected_map();
						location_index = 0; // reset location
						curr_map = map_num;
						//reset bomb and bullet
						bullet->set_total(6);
						bomb->set_total(6);
						reset_level(true);
						welcome_shown = true;
						break;
					}
					case (WelcomeDialog::KEYS):
						process_keys = true;
						break;
					case (WelcomeDialog::QUIT):
						quit = true;
						break;
					default:
						break;
				}

				delete(welcome_dialog);
				welcome_dialog = NULL;
				delete(welcome);
				welcome = NULL;
			}
		}
		else if(!intro_shown)
		{
			//map intro
			if(!intro)
				intro = new Intro(gc, Levels, map_num);

			intro->show(time_elapsed);

			if(events->keys[RE_Events::KEY_ACTION] ||
				events->keys[RE_Events::KEY_RETURN])
			{
				intro_shown = true;
				if(intro)
				{
					delete(intro);
					intro = NULL;
				}
			}
		}
		else if(!game_over)
		{
			//update scroll
			scroll_x = next_scroll_x;
			scroll_y = next_scroll_y;
			map->set_scroll(scroll_x, scroll_y);

			std::list<GameObject*>::iterator it;

			/*
			 *  Render Game
			 */
			//map->draw(); //draw all layers
			map->draw("background");
			map->draw("stairs");
			map->draw("walls");
			map->draw("platform");

			//render background objects
			for (
				it = objects.begin();
				it != objects.end();
				it++)
			{
				GameObject *cur = *it;
				if(!cur->foreground)
					cur->show(scroll_x, scroll_y);
			}

			map->draw("foreground");

			//render foreground objects
			for (
				it = objects.begin();
				it != objects.end();
				it++)
			{
				GameObject *cur = *it;
				if(cur->foreground)
					cur->show(scroll_x, scroll_y);
			}

			/*
			 *  Update objects state
			 */
			if(!paused)
			{
				for (
					it = objects.begin();
					it != objects.end();
					it++)
				{
					GameObject *cur = *it;
					cur->turn(time_elapsed);
				}
			}

			//draw score
			spr_score_back->set_frame(0);
			spr_score_back->draw(0, 0);

			std::ostringstream scoreSStream;

			if(score < 10)
				scoreSStream << "0000" << score;
			else if(score < 100)
				scoreSStream << "000" << score;
			else if(score < 1000)
				scoreSStream << "00" <<  score;
			else if(score < 10000)
				scoreSStream << "0" <<  score;
			else
				scoreSStream <<  score;

			score_font->draw_text(100, 0, scoreSStream.str());

			spr_score_bullets->set_frame(bullet->get_total());
			spr_score_bullets->draw(250, 0);

			spr_score_bombs->set_frame(bomb->get_total());
			spr_score_bombs->draw(380, 0);

			spr_score_lifes->set_frame(lifes);
			spr_score_lifes->draw(510, 0);

			if(paused)
				paused_font->draw(280, 220);


			if (player->is_dead())
			{
				lifes--;
				map_num = curr_map;
				submap_num = curr_submap;
				location_index = curr_location;

				bullet->set_total(6);
				bomb->set_total(6);
				reset_level();
			 	//std::cout << "lifes left: " << lifes << "\n";
				if(lifes <= 0)
					game_over = true;

				current_time = SDL_GetTicks(); //reset turn time
			}
			else if(change_level)
			{
				map_num = map->get_map(connection_index);
				submap_num = map->get_submap(connection_index);
				location_index = map->get_location(connection_index);
				//std::cout << "change level:"<<map_num<<";"<<submap_num<<";"<<location_index<<"\n";

				reset_level();
				current_time = SDL_GetTicks(); //reset turn time
				change_level = false;
			}

		}
		else if(process_score)
		{
			if(!score_dialog)
				score_dialog = new ScoreDialog(gc, global_resources, events, scores, score);

			score_dialog->update(time_elapsed);
			score_dialog->show(time_elapsed);

			if(score_dialog->is_commited())
			{
				delete(score_dialog);
				score_dialog = NULL;
				//reset
				process_score = false;
				lifes = 6;
				map_num = 1;
				submap_num = 1;
				curr_map = map_num;
				curr_submap = submap_num;
				welcome_shown = false;
				intro_shown = false;
				game_over = false;

				bonus_list.clear();
				score = 0;
				reset_level(true);
			}
		}
		else if(game_over)
		{
			if(menu_font->get_font_size() != 42)
				menu_font->resize(42);
			if(map_num < 1 || submap_num < 1)
			{
				menu_font->draw_text(140, 180, "Game Completed");
				menu_font->resize(24);
				menu_font->draw_text(130, 240, "...more levels in coming versions");

			}
			else
			{
				gameover_font->draw(200, 200);
				if(!sound->is_playing())
					sound->play(sfx_gameover);

			}
			if(events->keys[RE_Events::KEY_ACTION])
			{
				// send to c4a if enabled
				if(c4a > 0)
				{
					char buf[500];
					sprintf(buf, "./bin/sc so push rickyd pandora %i", score);
					//printf("%s\n", buf);
					int sys_ret = system(buf);
					if(sys_ret)
						std::cout << "rickyd: ./bin/sc system call returned "<< sys_ret<<"\n";
				}
				//check score
				if(scores->check_score(score))
				{
					//get player name
					process_score = true;
				}
				else
				{
					//reset
					lifes = 6;
					map_num = 1;
					submap_num = 1;
					curr_map = map_num;
					curr_submap = submap_num;
					welcome_shown = false;
					intro_shown = false;
					game_over = false;

					bonus_list.clear();
					score = 0;
					reset_level(true);
				}
			}
		}

		gc->flush();

		events->process();

		//if(events->check_mouse_click(&mouse_pointer))
		//{
		//	//just print the pointer coordinates for now
		//	std::cout << "mouse clicked at "<< mouse_pointer.x<<","<<mouse_pointer.y<<"\n";
		//}

		if(events->keys[RE_Events::KEY_QUIT])
		{
			if(!welcome_shown)
				quit = true;
			else
			{
				//reset
				lifes = 6;
				map_num = 1;
				submap_num = 1;
				curr_map = map_num;
				curr_submap = submap_num;
				welcome_shown = false;
				intro_shown = false;
				if(intro)
				{
					delete(intro);
					intro = NULL;
				}
				game_over = false;

				bonus_list.clear();
				score = 0;
				reset_level(true);
				//reset key press
				events->keys[RE_Events::KEY_QUIT] = false;
			}
		}
		if(events->keys[RE_Events::KEY_PAUSE] && pause_key_up)
		{
			paused = !paused; //toggle state
			pause_key_up = false; //key is down
		}
		else if (!events->keys[RE_Events::KEY_PAUSE] && !pause_key_up)
		{
			pause_key_up = true; //key is up
		}
	}

	Uint32 end_time = SDL_GetTicks();

	double average_fps =  ((double)frame_counter/(double)(end_time - start_time))*1000;

//	  std::cout << "total frames:" << frame_counter << "\n";
//	  std::cout << "total time (ms):" << end_time - start_time << "\n";
//	  std::cout << "average fps:" << average_fps << "\n";
}

GameObject* World::check_collision(GameObject::ObjectType type, GameObject* owner)
{
	std::list<GameObject*>::iterator it;
	//:collisions
	for (
		it = objects.begin();
		it != objects.end();
		it++)
	{
		GameObject *cur = *it;
		if((cur->get_type() == type || type == GameObject::any) && (cur != owner))
		{
			if(cur->check_box_collision(owner->get_collision_rect()))
			{
				cur->set_object_collision(owner); //used by bullet (must be reset in cur)
				owner->set_object_collision(cur);
				return cur;
			}
		}
	}

	return NULL;
}

GameObject* World::check_point_collision(GameObject::ObjectType type,
										RE_Point* point,
										GameObject* obj,
										bool reset_iter)
{
	std::list<GameObject*>::iterator it;
	std::list<GameObject*>::iterator curr_iter;

	if(obj && !reset_iter)
		curr_iter = obj->get_objects_iter();
	else
		curr_iter = objects.begin();

	if( curr_iter == objects.end())
	{
		curr_iter = objects.begin();
		return NULL;
	}

	//:collisions
	for (
		it = curr_iter; //start from current object iteration
		it != objects.end();
		it++)
	{
		GameObject *cur = *it;

		if(cur != obj && (cur->get_type() == type || type == GameObject::any))
		{
			if(cur->check_point_collision(point))
			{
				if(obj)
					obj->set_objects_iter(it); //update iterator
				return cur;
			}
		}
	}

	if(obj)
		obj->set_objects_iter(objects.begin()); //reset
	return NULL;
}

GameObject* World::check_box_collision(GameObject::ObjectType type,
										RE_Rect* box,
										GameObject* obj,
										bool reset_iter)
{
	//:collisions
	std::list<GameObject*>::iterator it;
	std::list<GameObject*>::iterator curr_iter;

	if(obj && !reset_iter)
		curr_iter = obj->get_objects_iter();
	else
		curr_iter = objects.begin();

	if( curr_iter == objects.end())
	{
		curr_iter = objects.begin();
		return NULL;
	}

	for (
		it = curr_iter; //start from current object iteration
		it != objects.end();
		it++)
	{
		GameObject *cur = *it;

		if(cur != obj && (cur->get_type() == type || type == GameObject::any))
		{
			if(cur->check_box_collision(box))
			{
				if(obj)
				{
					obj->set_objects_iter(it); //update iterator
				}
				return cur;
			}
		}
	}

	if(obj)
		obj->set_objects_iter(objects.begin()); //reset
	return NULL;
}
