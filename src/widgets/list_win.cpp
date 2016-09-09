/*
 * list_win.cpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: carlosfaruolo
 */

#include "list_win.hpp"
#include "scrollable_pane.hpp"

using std::string;
using WidgetsExtra::AbstractListWin;

AbstractListWin::AbstractListWin(WinBase* parent, Style style, Rect rect, Id id)
: BgrWin(parent, rect, null, drawBgrWinAsListWin, onMouseDown, onMouseMove, onMouseUp, calc_color(0xffffffff), id),
  padding(1,1), spacing(1),
  textRenderer(draw_ttf), textRendererCaseSelected(draw_blue_ttf),
  bgcolCaseSelected(calc_color(0xffA0D0E0)),
  selection(), preventRedrawOnClick(false), enableScrollingIfScrollablePaneParent(true), lastClickedIndex(0)
{}

AbstractListWin::~AbstractListWin() {}

void AbstractListWin::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, this->bgcol);

	const unsigned lineHeight = TTF_FontHeight(this->textRenderer->ttf_font);

	SDL_Rect selectedLineRect = SDL_Rect(); //area to draw selected item background
	selectedLineRect.w = this->tw_area.w; selectedLineRect.h = lineHeight;

	Point itemLocation = this->padding; //start after padding

	for(unsigned index = 0; index < this->size(); index++)
	{
		string itemStr = this->getAsString(index);
		if(selection.isSelected(index))
		{
			selectedLineRect.y = itemLocation.y;
			SDL_FillRect(this->win, &selectedLineRect, bgcolCaseSelected);
			this->textRendererCaseSelected->draw_string(this->win, itemStr.c_str(), itemLocation);
		}
		else
			this->textRenderer->draw_string(this->win, itemStr.c_str(), itemLocation);

		itemLocation.y += lineHeight + this->spacing;
	}
}

unsigned AbstractListWin::getListHeight()
{
	return this->padding.y + this->size() * (TTF_FontHeight(this->textRenderer->ttf_font) + this->spacing);
}

void AbstractListWin::clickList(const Point& point)
{
	 //if out of bounds, don't do anything
	if(point.x < padding.x or point.y < padding.y) return;

	//infer index by mouse position
	unsigned index = (point.y - padding.y) / (TTF_FontHeight(textRenderer->ttf_font) + spacing);

	//set selected item if inside range. if not, clear and leave
	if(index > this->size() - 1)
	{
		this->selection.clear();
		return;
	}

	if(SDL_GetModState() & KMOD_CTRL) // check if click-wise selection
	{
		if(SDL_GetModState() & KMOD_SHIFT) // check if click-wise, multi-selection
		{
			if(index > lastClickedIndex)
				this->selection.select(lastClickedIndex, index); // set range selected
			else
				this->selection.select(index, lastClickedIndex); // set range selected (inverted)
		}
		else // click-wise selection only
		{
			this->selection.toogle(index);
			this->lastClickedIndex = index;
		}
	}

	else if(SDL_GetModState() & KMOD_SHIFT) // check if multi selection
	{
		if(index > lastClickedIndex)
			this->selection.setSelected(lastClickedIndex, index); // set range selected
		else
			this->selection.setSelected(index, lastClickedIndex); // set range selected (inverted)
	}
	else // single selection
	{
		this->selection.setSelected(index);
		this->lastClickedIndex = index;
	}
}

void AbstractListWin::onMouseDown(Point point, int buttonNumber)
{
	if(this->enableScrollingIfScrollablePaneParent) ScrollablePane::scrollContentOnMouseWheel(this, buttonNumber);
	if(buttonNumber != SDL_BUTTON_LEFT) return; // only accepts left-button clicks
	this->clickList(point);
	if(preventRedrawOnClick) return;
	this->draw_blit_upd(); // redraw
}

void AbstractListWin::onMouseMove(Point point, int buttonNumber) {}  // by default does nothing

void AbstractListWin::onMouseUp(Point point, int buttonNumber) {}  // by default does nothing

// ################# static functions to be used when referencing AbstractListWin's as a BgrWin's.  #################
void AbstractListWin::drawBgrWinAsListWin(BgrWin* listWinAsBgrWin) 				{ static_cast<AbstractListWin*>(listWinAsBgrWin)->draw(); }
void AbstractListWin::onMouseDown(BgrWin* listWinAsBgrWin,int x,int y,int but) 	{ static_cast<AbstractListWin*>(listWinAsBgrWin)->onMouseDown(Point(x, y), but); }
void AbstractListWin::onMouseMove(BgrWin* listWinAsBgrWin,int x,int y,int but) 	{ static_cast<AbstractListWin*>(listWinAsBgrWin)->onMouseMove(Point(x, y), but); }
void AbstractListWin::onMouseUp(BgrWin* listWinAsBgrWin,int x,int y,int but) 	{ static_cast<AbstractListWin*>(listWinAsBgrWin)->onMouseUp(Point(x, y), but); }

