/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/
#include "re_listbox.h"
#include "tinyxml2.h"

RE_ListBox::RE_ListBox(RE_GameContext* pgc, std::string xml_path, RE_ResourceManager *resource, RE_Events* pevents)
	: RE_Resource(pgc, pevents) //pass the events to resource
{
	spr_background = new RE_Sprite(gc, xml_path+"/box/sprite", resource);
	input_font = new RE_Font(gc, xml_path+"/font", resource);

	up_box_rect = new RE_Rect(0, 0, spr_background->get_width(), UP_DOWN_HEIGHT);
	down_box_rect = new RE_Rect(0, spr_background->get_height() - UP_DOWN_HEIGHT, spr_background->get_width(), spr_background->get_height());
	list_box_rect = new RE_Rect(0, UP_DOWN_HEIGHT, spr_background->get_width(), spr_background->get_height()-UP_DOWN_HEIGHT);
	selected_index = 0;
	set_commited(false);
	turn_time = 0;
	scroll_item = 0;
	max_display_items = 4;
	item_height = 21;
}

RE_ListBox::~RE_ListBox()
{
	delete(spr_background);
	//for (
	//	std::vector<RE_ListBoxItem*>::iterator it = item_list.begin();
	//	it != item_list.end();
	//	it++)
	//{
	//	delete *it;
	//}
	item_list.clear();
	delete(input_font);
	delete(up_box_rect);
	delete(down_box_rect);
	delete(list_box_rect);
}

void RE_ListBox::add_item(RE_ListBoxItem new_item)
{
	item_list.push_back(new_item);
}

void RE_ListBox::enable_item(int index, bool enable)
{
	if(index >=0 && index < item_list.size())
		item_list[index].enabled = enable;
}

std::string RE_ListBox::get_text(int index)
{
	if(index >=0 && index < item_list.size())
		return item_list[index].text;
	else
		return "";
}

int RE_ListBox::get_value(int index)
{
	if(index >=0 && index < item_list.size())
		return item_list[index].value;
	else
		return 0;
}

bool RE_ListBox::is_enabled(int index)
{
	if(index >=0 && index < item_list.size())
		return false;
	else
		return item_list[index].enabled;
}

int RE_ListBox::set_selected_index(int index)
{
	if(index >=0 && index < item_list.size())
		selected_index = index;
	else
		selected_index = 0;

	if(selected_index >= scroll_item + max_display_items)
		scroll_item = selected_index - max_display_items + 1;
	else if(selected_index < scroll_item)
		scroll_item = selected_index;

	return selected_index;
}

void RE_ListBox::update(Uint32 time_elapsed)
{
	check_tab_event();

	turn_time = time_elapsed;

	if(has_focus())
	{
		if(events->keys[RE_Events::KEY_DOWN])
		{
			if((selected_index +1) < item_list.size())
				selected_index++;
			if(selected_index >= scroll_item + max_display_items)
				scroll_item = selected_index - max_display_items + 1;
			//reset
			events->keys[RE_Events::KEY_DOWN] = false;
		}
		else if(events->keys[RE_Events::KEY_UP])
		{
			if((selected_index - 1) >= 0)
				selected_index--;
			if(selected_index < scroll_item)
				scroll_item = selected_index;
			//reset
			events->keys[RE_Events::KEY_UP] = false;
		}
		else if (events->keys[RE_Events::KEY_RETURN])
		{
			set_commited(true);
			events->keys[RE_Events::KEY_RETURN] = false;
		}
	}

	//mouse_pointer only works when window is not deformed(window = gc)
	//so we need to convert it from window to gc resolution
	if(events->check_mouse_click(&mouse_pointer))
	{
		//double width_factor = (double) gc->get_width()/gc->get_window_width();
		//double height_factor = (double) gc->get_height()/gc->get_window_height();
		double width_factor = 1;
		double height_factor = 1;

		//convert mouse position(window) to gc resolution
		gc_pointer.x = (int) mouse_pointer.x * width_factor;
		gc_pointer.y = (int) mouse_pointer.y * height_factor;

		//check colision with up_box
		if(gc_pointer.is_inside(up_box_rect))
		{
			//std::cout << "mouse clicked in listbox up button\n";
			grab_focus();
			if(scroll_item > 0)
				scroll_item--;

		}
		else if(gc_pointer.is_inside(down_box_rect)) //check colision with down_box
		{
			//std::cout << "mouse clicked in listbox down button\n";
			grab_focus();
			if(item_list.size() > scroll_item + max_display_items)
				scroll_item++;
		}
		else if(gc_pointer.is_inside(list_box_rect)) //check colision with list items
		{
			grab_focus();
			int i=0;
			int max = item_list.size();
			if(max > max_display_items)
				max = max_display_items;

			for (i=0; i< max; i++)
			{
				if(gc_pointer.y < list_box_rect->Top() + (i+1) * item_height)
				{
					selected_index = scroll_item + i;
					break;
				}
			}

			//if(gc_pointer.y < list_box_rect->Top() + item_height)
			//{
			//	//std::cout << "mouse clicked in listbox item 1\n";
			//	selected_index = scroll_item;
			//}
			//else if(gc_pointer.y < list_box_rect->Top() + 2 * item_height)
			//{
			//	selected_index = scroll_item + 1;
			//}
			//else if(gc_pointer.y < list_box_rect->Top() + 3 * item_height)
			//{
			//	selected_index = scroll_item + 2;
			//}
			//else if(gc_pointer.y < list_box_rect->Top() + 4 * item_height)
			//{
			//	selected_index = scroll_item + 3;
			//}
		}

		//std::cout << "mouse pointer(real)(gc)):("<< mouse_pointer.x<<","<<mouse_pointer.y<<")("<<gc_pointer.x<<","<<gc_pointer.y<<")\n";
	}
}

void RE_ListBox::draw(int x, int y)
{

	item_height = input_font->get_text_height() + input_font->get_text_height()/2;
	//std::cout << "item_height="<< item_height<<"\n";

	cursor_pos = x + 10;
	if(label.size() > 0)
	{
		input_font->set_fg_color(255,255,255,0);
		input_font->set_bg_color(0,0,0,0);
		input_font->draw_text(x, y, label);
		y += input_font->get_text_height(); //draw input box below label
	}

	up_box_rect->Left(x);
	up_box_rect->Top(y);
	up_box_rect->Right(x+spr_background->get_width());
	up_box_rect->Bottom(y + UP_DOWN_HEIGHT);

	list_box_rect->Left(x);
	list_box_rect->Top(y + UP_DOWN_HEIGHT);
	list_box_rect->Right(x+spr_background->get_width());
	list_box_rect->Bottom(y + spr_background->get_height() - UP_DOWN_HEIGHT);

	down_box_rect->Left(x);
	down_box_rect->Top(y + spr_background->get_height() - UP_DOWN_HEIGHT);
	down_box_rect->Right(x+spr_background->get_width());
	down_box_rect->Bottom(y + spr_background->get_height());

	if(has_focus())
		spr_background->set_frame(1);
	else
		spr_background->set_frame(0);

	spr_background->draw(x, y);

	if(scroll_item > 0)
	{
		input_font->set_fg_color(0xa2,0xd4,0x53,0);
		input_font->set_bg_color(0,0,0,0);
		input_font->set_text("UP");
		int cursor_pos_x = up_box_rect->Left() + spr_background->get_width()/2 - input_font->get_text_width()/2;
		int cursor_pos_y = up_box_rect->Top() + UP_DOWN_HEIGHT/2 - input_font->get_text_height()/2;

		input_font->draw(cursor_pos_x , cursor_pos_y);
	}

	int max = item_list.size();
	if(max > scroll_item + max_display_items)
	{
		max = scroll_item + max_display_items;
		input_font->set_fg_color(0xa2,0xd4,0x53,0);
		input_font->set_bg_color(0,0,0,0);
		input_font->set_text("DOWN");
		int cursor_pos_x = down_box_rect->Left() + spr_background->get_width()/2 - input_font->get_text_width()/2;
		int cursor_pos_y = down_box_rect->Top() + UP_DOWN_HEIGHT/2 - input_font->get_text_height()/2;

		input_font->draw(cursor_pos_x , cursor_pos_y);
	}

	//draw list box items text
	y = list_box_rect->Top();

	for (int i = scroll_item; i < max; i++)
	{
		y += input_font->get_text_height()/2;

		if(y < (list_box_rect->Bottom() - input_font->get_text_height()))
		{
			if(item_list[i].enabled)
			{
				input_font->set_fg_color(255,255,255,0);
				input_font->set_bg_color(0,0,0,0);
			}
			else
			{
				input_font->set_fg_color(127,127,127,0);
				input_font->set_bg_color(0,0,0,0);
			}
			if(i == get_selected_index())
				input_font->set_bg_color(100,10,100,0);

			//use a fixed sized texture since we are coloring the background
			input_font->draw_text(cursor_pos, y, item_list[i].text, 280, input_font->get_text_height());
		}
		else
			break;

		//next line
		y += input_font->get_text_height();
	}



}
