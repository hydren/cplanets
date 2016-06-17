/*
 * list_win.cpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: carlosfaruolo
 */

#include "list_win.hpp"

#include "widgets_util.hpp"

using WidgetsExtra::ListWin;
using std::vector;

ListWin::ListWin(WinBase *parent, Style, Rect rect, Id id)
: BgrWin(parent, rect, null, drawBgrWinAsListWin, 0 /*down_cmd*/, 0 /*moved_cmd*/, 0 /*up_cmd*/,  calc_color(0xffffffff), id),
  model(new DefaultUIListModel()), selection(), padding(1,1), spacing(1), textRenderer(draw_ttf)
{}

void ListWin::setListData(const std::vector<std::string>& data)
{
	UIListModel* old = this->model;
	this->model = new DefaultUIListModel(data);
	if(old != null) delete old;
}

ListWin::~ListWin()
{
	delete model;
}

void ListWin::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, this->bgcol);

	vector<string> listItems = this->model->getStringfiedList();
	const unsigned lineHeight = TTF_FontHeight(this->textRenderer->ttf_font);

	Point itemLocation = this->padding; //start after padding
	foreach(string&, itemStr, vector<string>, listItems)
	{
		this->textRenderer->draw_string(this->win, itemStr.c_str(), itemLocation);
		itemLocation.y += lineHeight + this->spacing;
	}
}

void ListWin::drawBgrWinAsListWin(BgrWin* selfAsBgrWin)
{
	static_cast<ListWin*>(selfAsBgrWin)->draw();
}
