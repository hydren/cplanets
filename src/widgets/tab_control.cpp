/*
 * tab_control.cpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "tab_control.hpp"

#include <stdexcept>
#include <map>

#include "futil/futil.hpp"

using std::vector;
using std::map;

using SDL_util::TabController;

map<RExtButton*, TabController*> references; //xxx kludge to maintain references to TabController's

TabController::TabController(WinBase* parent)
: commonParent(parent), tabButtonsController(null)
{
	tabButtonsController = new ExtRButCtrl(Style(0, calc_color(0xe0e0e0)), onTabButtonClicked);
}

void TabController::addTab(Rect tabButtonRect, Label lab, BgrWin* content)
{
	this->tabButtons.push_back(this->tabButtonsController->add_extrbut(this->commonParent, tabButtonRect, lab, Id(this->tabButtons.size())));
	this->tabsPanels.push_back(content);
	references[this->tabButtons.back()] = this; //xxx kludge to maintain references to TabController's
}

/// Sets the active (visible) tab.
void TabController::setActiveTab(unsigned index)
{
	if(index > this->tabButtons.size() - 1) throw std::invalid_argument("invalid tab index");
	this->tabButtonsController->set_rbut(this->tabButtons[index]);
}

void TabController::setActiveTab(BgrWin* tabContent)
{
	int index = Collections::indexOf(tabsPanels, tabContent);
	if(index != -1)
		setActiveTab(index);
	// else throw std::invalid_argument; //should we?
}

void TabController::onTabButtonClicked(RExtButton* rbTab, bool is_act)
{
	TabController* controller = references[rbTab];
	foreach(BgrWin*, panel, vector<BgrWin*>, controller->tabsPanels)
	{
		panel->hide();
	}

	int index = Collections::indexOf(controller->tabButtons, rbTab);
	controller->tabsPanels[index]->show();
}


