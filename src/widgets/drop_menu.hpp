/*
 * drop_menu.hpp
 *
 *  Created on: 18 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_DROP_MENU_HPP_
#define WIDGETS_DROP_MENU_HPP_

#include "abstract_layout.hpp"

struct DropDownMenu extends CPlanetsGUI::Layout::Element
{
	CmdMenu* cmdMenu;
	virtual ~DropDownMenu();

	virtual Point getPosition() const;
	virtual void setPosition(Point position);
	virtual Rect getSize() const;
	virtual void setSize(Rect size);
	virtual bool isStretched() const;
	virtual bool operator == (const Element& b) const;
};

struct DropDownMenuFactory
{
	DropDownMenuFactory();
	~DropDownMenuFactory();
	enum Appearance { BUTTON, COMBOBOX, MENU };

	void setLabel(Label label);
	void setCallback(void (*menu_cmd)(RButWin*,int nr,int fire));
	void setAppearance(Appearance apre);
	void setSize(Rect rt);

	void addItem(Label label, short index=-1);
	Label itemAt(unsigned index);

	DropDownMenu* createAt(WinBase* pw, Point pt, Id id=0);
	DropDownMenu* createAt(WinBase* pw, Id id=0);
	friend class DropDownMenuButton;
	private:encapsulated;
};



#endif /* WIDGETS_DROP_MENU_HPP_ */
