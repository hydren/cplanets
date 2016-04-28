/*
 * tab_set.cpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "tab_set.hpp"

#include "widgets_util.hpp"
#include "futil/futil.hpp"

using SDL_util::TabSet;
using SDL_util::FlowLayout;

TabSet::TabSet(WinBase* parent, int x, int y, Rect maxSize)
: controller(parent),
  layout(x, y, maxSize)
{}

void TabSet::addTab(Label lab, BgrWin* content)
{
	controller.addTab(Rect(0,0,1,1), lab, content);
	RExtButton* tabBtn = controller.tabButtons.back();
	SDL_util::packLabeledComponent(tabBtn);
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

