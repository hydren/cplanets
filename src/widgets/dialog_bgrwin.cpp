/*
 * dialog_bgrwin.cpp
 *
 *  Created on: 11 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "dialog_bgrwin.hpp"

#include "widgets_util.hpp"

using WidgetsExtra::DialogBgrWin;
using std::string;

static const char *close_btn_xpm[] = {
"8 7 3 1",
"  c None",
"x c #101010",
"- c #e0e0e0",
"xx    xx",
" xx  xx ",
"  xxxx  ",
"   xx   ",
"  xxxx  ",
" xx  xx ",
"xx    xx"};

DialogBgrWin::DialogBgrWin(Rect bounds, string txt, void (*onClosed)(DialogBgrWin*), Style st)
: BgrWin(null, bounds, null, DialogBgrWin::draw, DialogBgrWin::dialog_mwin_down, mwin::move, mwin::up, 0),
  WinBaseWrapper(this),
  onClosedCallback(onClosed),
  titleStr(txt),
  titleBarArea(Rect(0, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) -2)),
  style(st),
  titleStrOffset(0),
  btnClose(this, Style(0,1), Rect(0, 0, titleBarArea.h-4, titleBarArea.h-4), Label(""), create_pixmap(close_btn_xpm), DialogBgrWin::close)
{
	this->validate();
}

DialogBgrWin::~DialogBgrWin()
{}

void DialogBgrWin::setVisible(bool visibleDesired)
{
	if(visibleDesired)
	{
		if(this->parent == null)
			this->bind();
		else if(this->hidden)
			this->show();
	}
	else if(this->parent != null && not this->hidden)
	{
		this->hide();
	}
}

void DialogBgrWin::setPosition(Point position)
{
	WinBaseWrapper::setPosition(position); //set bgrwin position as a WinBaseWrapper
	this->validate();
}

void DialogBgrWin::setPositionOnCenter()
{
	this->setPosition(Point(0.5 * (SDL_GetVideoSurface()->w - this->tw_area.w), 0.5 * (SDL_GetVideoSurface()->h - this->tw_area.h)));
}

void DialogBgrWin::bind()
{
	this->keep_on_top(); //binds to main window
	this->bgcol = parent->bgcol; //inherit background color
	this->draw_blit_recur();
	this->upd();
}

void DialogBgrWin::validate()
{
	//shift properly all components inside it
	titleBarArea = Rect(1, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) - 2);
	setComponentPosition(&btnClose, titleBarArea.w - titleBarArea.h + 2, 2, true);
	titleStrOffset = 0.5 * (titleBarArea.h - TTF_FontHeight(draw_title_ttf->ttf_font));
}

void DialogBgrWin::draw(BgrWin* bwSelf)
{
	DialogBgrWin* self = static_cast<DialogBgrWin*>(bwSelf);

	self->init_gui();
	self->draw_raised(0, self->bgcol, true);

	if(self->style.st == 0)	switch(self->style.param)
	{
			case 0: Color5::GradientBlue.draw_gradient(self, self->titleBarArea); break;
			case 1: Color5::GradientGrey.draw_gradient(self, self->titleBarArea); break;
			case 2: Color5::GradientGreen.draw_gradient(self, self->titleBarArea); break;
			case 3: Color5::GradientWheat.draw_gradient(self, self->titleBarArea); break;
			case 4: Color5::GradientRose.draw_gradient(self, self->titleBarArea); break;
			case 5: Color5::GradientDarkBlue.draw_gradient(self, self->titleBarArea); break;
			case 6: Color5::GradientDarkGrey.draw_gradient(self, self->titleBarArea); break;
	}
	else
	{
		SDL_FillRect(self->win, &self->titleBarArea, calc_color(self->style.param));
	}

	draw_title_ttf->draw_string(self->win, self->titleStr.c_str(), Point(4, self->titleStrOffset));
}

void DialogBgrWin::close(Button* btn)
{
	DialogBgrWin* self = static_cast<DialogBgrWin*>(btn->parent);
	self->setVisible(false);
	if(self->onClosedCallback != null)
		self->onClosedCallback(self);
}

void DialogBgrWin::buttonCallbackCloseParentDialogBgrWin(Button* btn)
{ DialogBgrWin::close(btn); }

void DialogBgrWin::dialog_mwin_down(BgrWin* bgr,int x,int y,int but)
{
	Rect& titleBarArea = static_cast<DialogBgrWin*>(bgr)->titleBarArea;
	if(x > titleBarArea.x && x < titleBarArea.x + titleBarArea.w
	&& y > titleBarArea.y && y < titleBarArea.y + titleBarArea.h)
		mwin::down(bgr, x, y, but);
}
