/*
 * drop_menu.cpp
 *
 *  Created on: 18 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "drop_menu.hpp"

using std::vector;

// DropDownMenu ===========================================================================================

DropDownMenu::~DropDownMenu()
{
	//fixme needs a way to delete the buttons inside
	delete cmdMenu;
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

// DropDownMenuButton =========================================================================================================

struct DropDownMenuButton extends Button
{
	DropDownMenu* menu;
	DropDownMenuFactory::Implementation* factory;
	DropDownMenuButton(WinBase *pw, Style st, Rect rt, Label lab, void (*someCmd)(Button*), Id id=0)
	: Button(pw, st, rt, lab, someCmd, id), menu(null), factory(null) {}
	virtual ~DropDownMenuButton(){ delete factory; }
};

// DropDownMenuFactory (encapsulation) ==========================================================================================

encapsulation(DropDownMenuFactory)
{
	Implementation()
	: pw(null), apre(), rt(), lb(""), id(0), items(), menuItemCmd(null) {}

	WinBase* pw;
	Appearance apre;
	Rect rt;
	Label lb;
	Id id;
	vector<Label> items;
	void (*menuItemCmd)(RButWin*,int nr,int fire);

	//do the steps needed to initialize the CmdMenu wrapped inside the DropDownMenu
	static void buildCallback(Button* btn)
	{
		DropDownMenu* menu = ((DropDownMenuButton*) btn)->menu; //deference the menu
		DropDownMenuFactory::Implementation* factory = ((DropDownMenuButton*) btn)->factory; //deference the menu

		menu->cmdMenu->init(factory->rt.w, factory->items.size(), factory->menuItemCmd);
		for(unsigned i = 0; i < factory->items.size(); i++)
			menu->cmdMenu->add_mbut(factory->items[i]);
	}

	Style toStyle(Appearance ap)
	{
		switch(ap)
		{
			default:
			case BUTTON: return Style(0,0);
			case COMBOBOX: return Style(4,0);
			case MENU: return Style(2,1,20);
		}
	}

	//crafts a button that initializes a CmdMenu with this' parameters
	DropDownMenuButton* createButton(DropDownMenu* owner)
	{
		DropDownMenuButton* ddmb = new DropDownMenuButton(pw, toStyle(apre), rt, lb, buildCallback, id);
		ddmb->menu = owner;
		ddmb->factory = this;
		return ddmb;
	}
};

// DropDownMenuFactory =============================================================================================================

DropDownMenuFactory::DropDownMenuFactory()
: implementation(new Implementation()) {}

DropDownMenuFactory::~DropDownMenuFactory()
{
	delete implementation;
}

void DropDownMenuFactory::setLabel(Label lbl)
{
	this->implementation->lb = lbl;
}

void DropDownMenuFactory::setCallback(void (*menu_cmd)(RButWin*,int nr,int fire))
{
	this->implementation->menuItemCmd = menu_cmd;
}

void DropDownMenuFactory::setAppearance(Appearance apre)
{
	this->implementation->apre = apre;
}

void DropDownMenuFactory::setSize(Rect rt)
{
	this->implementation->rt = rt;
}

void DropDownMenuFactory::addItem(Label label, short index)
{
	if(index == -1)
		this->implementation->items.push_back(label);
	else
		this->implementation->items.insert(this->implementation->items.begin() + index, label);
}

Label DropDownMenuFactory::itemAt(unsigned index)
{
	return this->implementation->items[index];
}

DropDownMenu* DropDownMenuFactory::createAt(WinBase* pw, Point pt, Id id)
{
	this->implementation->rt.x = pt.x;
	this->implementation->rt.y = pt.y;
	return this->createAt(pw, id);
}

DropDownMenu* DropDownMenuFactory::createAt(WinBase* pw, Id id)
{
	this->implementation->pw = pw;
	this->implementation->id = id;
	DropDownMenu* ddmenu = new DropDownMenu();
	ddmenu->cmdMenu = new CmdMenu(this->implementation->createButton(ddmenu));
	this->implementation = new Implementation(); //the previous is being used by the DropDownMenu
	return ddmenu;
}


