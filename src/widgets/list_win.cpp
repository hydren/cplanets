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
  bgcolCaseSelected(calc_color(0xffA0D0E0))
{}

void ListWin::setListData(const std::vector<std::string>& data, bool redrawImmediately)
{
	this->setListModel(new DefaultUIListModel(data), redrawImmediately);
}

void ListWin::setListModel(UIListModel* model, bool redrawImmediately, bool deletePrevious)
{
	UIListModel* previousModel = this->model;

	this->model = model;
	this->selection.clear();
	this->selection.insert(this->selection.begin(), model->size(), false);

	if(sdl_running and redrawImmediately) this->draw_blit_upd();
	if(deletePrevious and previousModel != null) delete previousModel;
}

ListWin::~ListWin()
{
	delete model;
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
		if(selection[index] == true)
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

void ListWin::onMouseDown(Point point, int buttonNumber)
{
	 //if out of bounds, don't do anything
	if(point.x < padding.x or point.y < padding.y) return;

	 //clears the selection
	this->selection.assign(this->selection.size(), false);

	//infer index by mouse position
	unsigned index = (point.y - padding.y) / (TTF_FontHeight(textRenderer->ttf_font) + spacing);

	 //set selected item if inside range
	if(index < this->model->size())
		this->selection[index].flip();

	 //redraw
	this->draw_blit_upd();
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
