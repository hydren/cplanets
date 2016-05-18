/*
 * scrollable_pane.cpp
 *
 *  Created on: 17 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "scrollable_pane.hpp"

#include "widgets_util.hpp"

using WidgetsExtra::ScrollablePane;

ScrollablePane::ScrollablePane(WinBase* parent, Style style, Rect bounds, Uint32 bgColor, Id id)
: WinBase(parent, null, bounds.x, bounds.y, bounds.w, bounds.h, bgColor, id),
  offset(0, 0),
  scrollbarThickness(style.param != 0 ? style.param : ScrollablePane::DEFAULT_SCROLLBAR_THICKNESS),
  content(this, Rect(0, 0, bounds.w - scrollbarThickness, bounds.h - scrollbarThickness), null, WidgetsExtra::drawDefaultBgrWin, null, null, null, bgColor, id),
  scrollbarHorizontal(this, Style(1,0,5), Rect(0, bounds.h - scrollbarThickness, bounds.w - scrollbarThickness, scrollbarThickness), content.tw_area.w, ScrollablePane::hscrollbarCallback, id),
  scrollbarVertical(this, Style(1,0,5), Rect(bounds.w - scrollbarThickness, 0, scrollbarThickness, bounds.h - scrollbarThickness), content.tw_area.h, ScrollablePane::vscrollbarCallback, id)
{}

ScrollablePane::~ScrollablePane()
{}

void ScrollablePane::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, this->bgcol); //clears the screen
}

#include <iostream>

void ScrollablePane::updateOffset(int dx, int dy)
{
	offset.x += dx; offset.y += dy;

	content.move(dx, dy);
	content.draw_blit_recur();
	content.blit_upd();

	std::cout << "dx=" << dx << ", dy=" << dy << std::endl;
	std::cout << "offset.x=" << offset.x << ", offset.y=" << offset.y << std::endl;
	std::cout << "content.area.x=" << content.area.x << ", content.area.y=" << content.area.y << std::endl;
}

void ScrollablePane::setOffset(int x, int y)
{
	this->updateOffset(x - offset.x, y - offset.y);
}

void ScrollablePane::widen(int dx, int dy)
{
	WinBase::widen(dx, dy);
	scrollbarVertical.move(dx, 0); scrollbarVertical.widen(0, dy);
	scrollbarHorizontal.move(0, dy); scrollbarHorizontal.widen(dx, 0);
}

void ScrollablePane::widenContent(int dx, int dy)
{
	content.widen(dx, dy);
	scrollbarHorizontal.set_range(content.tw_area.w);
	scrollbarVertical.set_range(content.tw_area.h);
}

void ScrollablePane::setScrollbarVerticalVisible(bool visibleDesired)
{
	if(visibleDesired && scrollbarVertical.hidden)
	{
		scrollbarVertical.hidden = false;
		scrollbarHorizontal.widen(-scrollbarThickness, 0);
		scrollbarHorizontal.set_range(scrollbarVertical.range);
	}
	else if(not visibleDesired && not scrollbarVertical.hidden)
	{
		scrollbarVertical.hidden = true;
		scrollbarHorizontal.widen(scrollbarThickness, 0);
		scrollbarHorizontal.set_range(scrollbarVertical.range);
	}
}

void ScrollablePane::setScrollbarHorizontalVisible(bool visibleDesired)
{
	if(visibleDesired && scrollbarHorizontal.hidden)
	{
		scrollbarHorizontal.hidden = false;
		scrollbarVertical.widen(0, -scrollbarThickness);
		scrollbarVertical.set_range(scrollbarVertical.range);
	}
	else if(not visibleDesired && not scrollbarVertical.hidden)
	{
		scrollbarHorizontal.hidden = true;
		scrollbarVertical.widen(0, scrollbarThickness);
		scrollbarVertical.set_range(scrollbarVertical.range);
	}
}

void ScrollablePane::hscrollbarCallback(HScrollbar* bar, int val, int range)
{
	ScrollablePane* self = static_cast<ScrollablePane*>(bar->parent);
	self->setOffset(-val, self->offset.y);
}


void ScrollablePane::vscrollbarCallback(VScrollbar* bar, int val, int range)
{
	ScrollablePane* self = static_cast<ScrollablePane*>(bar->parent);
	self->setOffset(self->offset.x, -val);
}
