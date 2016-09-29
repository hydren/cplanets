/*
 * icon_button.cpp
 *
 *  Created on: 29 de set de 2016
 *      Author: carlosfaruolo
 */

#include "icon_button.hpp"

#include "widgets_util.hpp"

using WidgetsExtra::IconButton;

IconButton::IconButton(WinBase *parent, Style style, Rect bounds, Label label, SDL_Surface* icon, void (*cmd)(Button*), Id id)
: Button(parent, style, bounds, label, cmd, id),
  icon(icon)
{}

IconButton::~IconButton()
{
	SDL_FreeSurface(this->icon);
}

void IconButton::pack(int height)
{
	if(label.render_t->text_width(label.str) < icon->w)
	{
		int properWidth = icon->w + 4;
		int properHeight = height != 0? height : TTF_FontHeight(label.render_t->ttf_font) + 4;
		widen(properWidth - tw_area.w, properHeight - tw_area.h);
	}
	else
		packLabeledComponent(this, height);
}

void IconButton::draw()
{
	init_gui();
	Button::draw();
	if(icon == null) return;

	SDL_Rect dstrect;
	if(style.param2 == ICON_CENTER || style.param2 == ICON_CENTER_DOWN || style.param2 == ICON_CENTER_UP)
		dstrect.x = (tw_area.w - icon->w)/2;
	else if(style.param2 == ICON_RIGHT || style.param2 == ICON_RIGHT_DOWN || style.param2 == ICON_RIGHT_UP)
		dstrect.x = tw_area.w - icon->w;

	if(style.param2 == ICON_CENTER || style.param2 == ICON_LEFT || style.param2 == ICON_RIGHT)
		dstrect.y = (tw_area.h - icon->h)/2;
	else if(style.param2 == ICON_CENTER_DOWN || style.param2 == ICON_LEFT_DOWN || style.param2 == ICON_RIGHT_DOWN)
		dstrect.y = tw_area.h - icon->h;

	SDL_BlitSurface(icon, null, win, &dstrect);
}
