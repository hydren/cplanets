/*
 * widgets_util.cpp
 *
 *  Created on: 27 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "widgets_util.hpp"

#include "futil/futil.hpp"

void SDL_util::setComponentPosition(WinBase* component, Point& position)
{
	setComponentPosition(component, position.x, position.y);
}

void SDL_util::setComponentPosition(WinBase* component, int x, int y)
{
	setComponentPositionX(component, x);
	setComponentPositionY(component, y);
}

void SDL_util::setComponentPositionX(WinBase* component, int x)
{
	component->area.x = x;
	component->tw_area.x = x;
	component->title_area.x = x;
	component->title_top.x = x;
	if (component->parent)
	{
		component->tw_area.x=component->area.x+component->parent->tw_area.x;
	}
}

void SDL_util::setComponentPositionY(WinBase* component, int y)
{
	component->area.y = y;
	component->tw_area.y = y;
	component->title_area.y = y-17;
	component->title_top.y = y-17;
	if (component->parent)
	{
		component->tw_area.y=component->area.y+component->parent->tw_area.y;
	}
}

void SDL_util::packLabeledComponent(WinBase* wb, Label& lbl, unsigned height, int padding)
{
	int properWidth = lbl.render_t->text_width(lbl.str) + 2*padding;
	int properHeight = height > 0? height : TTF_FontHeight(lbl.render_t->ttf_font) + 2*padding;  //if height is 0, use font height plus padding instead
	wb->widen(properWidth - wb->tw_area.w, properHeight - wb->tw_area.h);
}

void SDL_util::packLabeledComponent(Button* btn, unsigned height, int padding)
{
	packLabeledComponent(btn, btn->label, height, padding);
}

void SDL_util::packLabeledComponent(CheckBox* btn, unsigned height, int padding)
{
	int properHeight = height > 0? height : 14;  //if height is 0, use size 14 (check symbol size defined as pixmap)
	int properWidth = btn->label.render_t->text_width(btn->label.str) + 2*padding + properHeight;
	btn->widen(properWidth - btn->tw_area.w, properHeight - btn->tw_area.h);
}

void SDL_util::packLabeledComponent(RExtButton* btn, unsigned height, int padding)
{
	packLabeledComponent(btn, btn->label, height, padding);
}

void SDL_util::drawDefaultBgrWin(BgrWin* bgrWin)
{
	bgrWin->init_gui();
	SDL_FillRect(bgrWin->win, null, bgrWin->parent->bgcol); //clears the screen
}
