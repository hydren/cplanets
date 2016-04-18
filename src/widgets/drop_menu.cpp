/*
 * drop_menu.cpp
 *
 *  Created on: 18 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "drop_menu.hpp"

struct DropDownMenuButton extends Button
{
	DropDownMenu* menu;
	DropDownMenuButton(DropDownMenu* menu, WinBase *pw, Style st, Rect rt, Label lab, void (*someCmd)(Button*), Id id=0)
	: Button(pw, st, rt, lab, someCmd, id), menu(menu) {}
	virtual ~DropDownMenuButton(){} //just to shut up
};


DropDownMenu::DropDownMenu(WinBase *pw,Style st,Rect rt,Label lab,Id id)
: cmdMenu(null),
  auxButton(new DropDownMenuButton(this, pw, st, rt, lab, DropDownMenu::buildCallback, id)), auxMenuCmd(null)
{}

DropDownMenu::~DropDownMenu(){}

void DropDownMenu::addItem(Label label)
{
	this->auxOptions.push_back(label);
}

void DropDownMenu::setCallback(void (*menu_cmd)(RButWin*,int nr,int fire))
{
	this->auxMenuCmd = menu_cmd;
}

void DropDownMenu::build()
{
	this->cmdMenu = new CmdMenu(auxButton);
}

void DropDownMenu::buildCallback(Button* btn)
{
	DropDownMenu* menu = ((DropDownMenuButton*) btn)->menu;
	menu->cmdMenu->init(menu->auxButton->tw_area.w, menu->auxOptions.size(), menu->auxMenuCmd);
	for(unsigned i = 0; i < menu->auxOptions.size(); i++)
		menu->cmdMenu->add_mbut(menu->auxOptions[i]);
}

Point DropDownMenu::getPosition() const
{
	return cmdMenu->src->area;
}

void DropDownMenu::setPosition(Point position)
{
	//todo
}

Rect DropDownMenu::getSize() const
{
	return cmdMenu->src->tw_area;
}

void DropDownMenu::setSize(Rect size)
{
	//todo
}

bool DropDownMenu::isStretched() const
{
	return false; //maybe true afterwards
}

bool DropDownMenu::operator == (const Element& b) const
{
	return &b == this;
}
