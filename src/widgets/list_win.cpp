/*
 * list_win.cpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: carlosfaruolo
 */

#include "list_win.hpp"

#include "widgets_util.hpp"
#include "SDL_util.hpp"

using WidgetsExtra::ListWin;
using WidgetsExtra::DefaultUIListModel;

using std::vector;


ListWin::ListWin(WinBase *parent, Style, Rect rect, Id id)
: BgrWin(parent, rect, null, drawBgrWinAsListWin, onMouseDown, onMouseMove, onMouseUp, calc_color(0xffffffff), id),
  model(new DefaultUIListModel()), selection(),
  padding(1,1), spacing(1),
  textRenderer(draw_ttf), textRendererCaseSelected(draw_blue_ttf),
  bgcolCaseSelected(calc_color(0xffA0D0E0)),
  preventRedrawOnClick(false)
{}

ListWin::~ListWin()
{
	delete model;
}

void ListWin::setListData(const std::vector<std::string>& data, bool redrawImmediately)
{
	this->setListModel(new DefaultUIListModel(data), redrawImmediately);
}

void ListWin::setListModel(UIListModel* model, bool redrawImmediately, bool deletePrevious)
{
	UIListModel* previousModel = this->model;

	this->model = model;
	this->selection.clear();
	this->selection.fit(model->size());

	if(sdl_running and redrawImmediately) this->draw_blit_upd();
	if(deletePrevious and previousModel != null) delete previousModel;
}

void ListWin::updateListData(void* data, bool redrawImmediately)
{
	this->model->updateData(data);
	this->selection.clear();
	this->selection.fit(model->size());
	if(sdl_running and redrawImmediately) this->draw_blit_upd();
}

void ListWin::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, this->bgcol);

	const unsigned lineHeight = TTF_FontHeight(this->textRenderer->ttf_font);

	SDL_Rect selectedLineRect = SDL_Rect(); //area to draw selected item background
	selectedLineRect.w = this->tw_area.w; selectedLineRect.h = lineHeight;

	Point itemLocation = this->padding; //start after padding

	for(unsigned index = 0; index < model->size(); index++)
	{
		string itemStr = model->getStringfiedElementAt(index);
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

unsigned ListWin::getListHeight()
{
	return this->padding.y + this->model->size() * (TTF_FontHeight(this->textRenderer->ttf_font) + this->spacing);
}

void ListWin::clickList(const Point& point)
{
	 //if out of bounds, don't do anything
	if(point.x < padding.x or point.y < padding.y) return;

	//infer index by mouse position
	unsigned index = (point.y - padding.y) / (TTF_FontHeight(textRenderer->ttf_font) + spacing);

	//set selected item if inside range. if not, clear and leave
	if(index > this->model->size() - 1)
	{
		this->selection.clear();
		return;
	}

	if(SDL_GetModState() & KMOD_CTRL) // check if click-wise selection
	{
		if(SDL_GetModState() & KMOD_SHIFT) // check if click-wise, multi-selection
		{
			//todo needs to remember last selection
		}
		else // click-wise selection only
		{
			this->selection.toogle(index);
		}
	}

	else if(SDL_GetModState() & KMOD_SHIFT) // check if multi selection
	{
		if(index > this->selection.getSelected())
			this->selection.setSelected(this->selection.getSelected(), index); // set range selected
		else
			this->selection.setSelected(index, this->selection.getSelected()); // set range selected (inverted)
	}
	else // single selection
	{
		this->selection.setSelected(index);
	}
}

void ListWin::onMouseDown(Point point, int buttonNumber)
{
	if(buttonNumber != SDL_BUTTON_LEFT) return; // only accepts left-button clicks
	this->clickList(point);
	if(preventRedrawOnClick) return;
	this->draw_blit_upd(); // redraw
}

void ListWin::onMouseMove(Point point, int buttonNumber) {}  // by default does nothing

void ListWin::onMouseUp(Point point, int buttonNumber) {}  // by default does nothing

// ++++++++++++++  to be used when referencing ListWin's as a BgrWin's.  ++++++++++++++

void ListWin::drawBgrWinAsListWin(BgrWin* listWinAsBgrWin)
{
	static_cast<ListWin*>(listWinAsBgrWin)->draw();
}

void ListWin::onMouseDown(BgrWin* listWinAsBgrWin,int x,int y,int but)
{
	static_cast<ListWin*>(listWinAsBgrWin)->onMouseDown(Point(x, y), but);
}

void ListWin::onMouseMove(BgrWin* listWinAsBgrWin,int x,int y,int but)
{
	static_cast<ListWin*>(listWinAsBgrWin)->onMouseMove(Point(x, y), but);
}

void ListWin::onMouseUp(BgrWin* listWinAsBgrWin,int x,int y,int but)
{
	static_cast<ListWin*>(listWinAsBgrWin)->onMouseUp(Point(x, y), but);
}
