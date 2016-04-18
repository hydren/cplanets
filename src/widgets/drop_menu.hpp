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

	DropDownMenu(WinBase *pw,Style st,Rect rt,Label lab,Id id=0);
	virtual ~DropDownMenu();
	void addItem(Label label);
	void setCallback(void (*menu_cmd)(RButWin*,int nr,int fire));
	void build();

	virtual Point getPosition() const;
	virtual void setPosition(Point position);
	virtual Rect getSize() const;
	virtual void setSize(Rect size);
	virtual bool isStretched() const;
	virtual bool operator == (const Element& b) const;

	private:
	Button* auxButton;
	std::vector<Label> auxOptions;
	void (*auxMenuCmd)(RButWin*,int nr,int fire);
	static void buildCallback(Button* btn);
};



#endif /* WIDGETS_DROP_MENU_HPP_ */
