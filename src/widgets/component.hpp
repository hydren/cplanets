/*
 * component.hpp
 *
 *  Created on: 13 de abr de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_COMPONENT_HPP_
#define WIDGETS_COMPONENT_HPP_

#include "futil/futil.hpp"
#include "SDL_widgets/SDL_widgets.h"

struct UIComponent
{
	virtual ~UIComponent();
	virtual void setComponentPosition(Point& pos) abstract;
	virtual Point getComponentPosition() abstract;
};

struct WinBaseWrapper extends UIComponent //maybe extend WinBase as well?
{
	WinBaseWrapper(WinBase* base);
	virtual ~WinBaseWrapper();
	virtual Point getComponentPosition();

	virtual void setComponentPosition(Point& position);
	virtual void setComponentPosition(int x, int y);
	virtual void setComponentPositionX(int x);
	virtual void setComponentPositionY(int y);

	private:
	WinBase* base;
};


#endif /* WIDGETS_COMPONENT_HPP_ */
