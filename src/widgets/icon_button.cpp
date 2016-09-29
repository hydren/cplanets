/*
 * icon_button.cpp
 *
 *  Created on: 29 de set de 2016
 *      Author: carlosfaruolo
 */

#include "icon_button.hpp"

using WidgetsExtra::IconButton;

IconButton::IconButton(WinBase *parent, Style style, Rect bounds, Label label, SDL_Surface* icon, void (*cmd)(Button*), Id id)
: Button(parent, style, bounds, label, cmd, id),
  icon(icon)
{}

IconButton::~IconButton()
{
	SDL_FreeSurface(this->icon);
}

void IconButton::draw()
{
	Button::draw();
	SDL_Rect* dstrect = null;
	switch(style.param2)
	{
		case ICON_CENTER:
		{
			dstrect = rp((tw_area.w - icon->w)/2, (tw_area.h - icon->h)/2 + (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_LEFT:
		{
			dstrect = rp(0, (tw_area.h - icon->h)/2 + (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_RIGHT:
		{
			dstrect = rp((tw_area.w - icon->w), (tw_area.h - icon->h)/2 + (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_CENTER_UP:
		{
			dstrect = rp((tw_area.w - icon->w)/2, (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_LEFT_UP:
		{
			dstrect = rp(0, (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_RIGHT_UP:
		{
			dstrect = rp((tw_area.w - icon->w), (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_CENTER_DOWN:
		{
			dstrect = rp((tw_area.w - icon->w)/2, (tw_area.h - icon->h) + (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_LEFT_DOWN:
		{
			dstrect = rp(0, (tw_area.h - icon->h) + (is_down? 1 : 0),0,0);
			break;
		}
		case ICON_RIGHT_DOWN:
		{
			dstrect = rp((tw_area.w - icon->w), (tw_area.h - icon->h) + (is_down? 1 : 0),0,0);
			break;
		}
	}
	SDL_BlitSurface(icon, null, win, dstrect);
}
