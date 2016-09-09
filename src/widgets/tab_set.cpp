/*
 * tab_set.cpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "tab_set.hpp"

#include "widgets_util.hpp"

using WidgetsExtra::TabSet;
using WidgetsExtra::FlowLayout;

TabSet::TabSet(WinBase* parent, int x, int y, unsigned w, unsigned h)
: commonParent(parent),
  controller(),
  layout(x, y, w, h)
{}

void TabSet::addTab(Label lab, BgrWin* content)
{
	controller.addTab(commonParent, Rect(0,0,1,1), lab, content);
	RExtButton* tabBtn = controller.tabButtons.back();
	WidgetsExtra::packLabeledComponent(tabBtn);
	layout.addComponent(tabBtn);
	layout.pack();
}

void TabSet::setActiveTab(BgrWin* tabContent)
{
	controller.setActiveTab(tabContent);
}

void TabSet::setActiveTab(unsigned index)
{
	controller.setActiveTab(index);
}

void TabSet::drawTabStyleBgrWin(BgrWin* bgrWin)
{
	WidgetsExtra::drawBgrWin(bgrWin);
	lineColor(bgrWin->win, 2, 1, bgrWin->tw_area.w - 2, 1, 0x606060ff); //top line
	lineColor(bgrWin->win, 2, bgrWin->tw_area.h - 1, bgrWin->tw_area.w - 2, bgrWin->tw_area.h - 1, 0x606060ff); //bottom line
	lineColor(bgrWin->win, 1, 2, 1, bgrWin->tw_area.h - 2, 0x606060ff); //leftmost line
	lineColor(bgrWin->win, bgrWin->tw_area.w - 1, 2, bgrWin->tw_area.w - 1, bgrWin->tw_area.h - 2, 0x606060ff); //rightmost line
}
