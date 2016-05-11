/*
 * dialog_bgrwin.cpp
 *
 *  Created on: 11 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "dialog_bgrwin.hpp"

#include "widgets_util.hpp"

using WidgetsExtra::DialogBgrWin;

DialogBgrWin::DialogBgrWin(Rect bounds, string txt, void (*onClosed)(DialogBgrWin*))
: BgrWin(null, bounds, null, DialogBgrWin::draw, DialogBgrWin::custom_mwin_down, mwin::move, mwin::up, 0),
  WinBaseWrapper(this),
  onClosedCallback(onClosed),
  titleStr(txt),
  titleBarArea(Rect(0, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) -2)),
  titleStrOffset(0),
  btnClose(this, Style(0,1), Rect(0, 0, titleBarArea.h-4, titleBarArea.h-4), "X", DialogBgrWin::close)
{
	this->setPosition(area);
	this->keep_on_top(); //binds to main window
	this->bgcol = parent->bgcol; //inherit background color
	this->draw_blit_recur();
	this->upd();
}

DialogBgrWin::~DialogBgrWin()
{}

void DialogBgrWin::setVisible(bool visible)
{
	if(visible && this->hidden) this->show();
	else if(not visible && not this->hidden) this->hide();
}

void DialogBgrWin::setPosition(Point position)
{
	WinBaseWrapper::setPosition(position); //set bgrwin position as a WinBaseWrapper

	//shift properly all components inside it
	titleBarArea = Rect(1, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) - 2);
	setComponentPosition(&btnClose, Point(titleBarArea.w - titleBarArea.h + 2, 2));
	titleStrOffset = 0.5 * (titleBarArea.h - TTF_FontHeight(draw_title_ttf->ttf_font));
}

void DialogBgrWin::setPositionOnCenter()
{
	setPosition(Point(SDL_GetVideoSurface()->w*0.5-this->tw_area.w*0.5, SDL_GetVideoSurface()->h*0.5-this->tw_area.h*0.5));
}

void DialogBgrWin::draw(BgrWin* bwSelf)
{
	DialogBgrWin* self = static_cast<DialogBgrWin*>(bwSelf);

	self->init_gui();
	self->draw_raised(0, self->bgcol, true);

	Color5::GradientBlue.draw_gradient(self, self->titleBarArea);

	draw_title_ttf->draw_string(self->win, self->titleStr.c_str(), Point(4, self->titleStrOffset));
}

void DialogBgrWin::close(Button* btn)
{
	DialogBgrWin* self = static_cast<DialogBgrWin*>(btn->parent);
	self->setVisible(false);
	if(self->onClosedCallback != null)
		self->onClosedCallback(self);
}

void DialogBgrWin::custom_mwin_down(BgrWin* bgr,int x,int y,int but)
{
	Rect& titleBarArea = static_cast<DialogBgrWin*>(bgr)->titleBarArea;
	if(x > titleBarArea.x && x < titleBarArea.x + titleBarArea.w
	&& y > titleBarArea.y && y < titleBarArea.y + titleBarArea.h)
		mwin::down(bgr, x, y, but);
}
