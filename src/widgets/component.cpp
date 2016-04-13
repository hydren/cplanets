/*
 * component.cpp
 *
 *  Created on: 13 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "component.hpp"

UIComponent::~UIComponent(){}

WinBaseWrapper::WinBaseWrapper(WinBase* base)
: base(base)
{}

WinBaseWrapper::~WinBaseWrapper(){}

Point WinBaseWrapper::getComponentPosition()
{
	return base->area;
}

void WinBaseWrapper::setComponentPosition(Point& position)
{
	setComponentPosition(position.x, position.y);
}

void WinBaseWrapper::setComponentPosition(int x, int y)
{
	setComponentPositionX(x);
	setComponentPositionY(y);
}

void WinBaseWrapper::setComponentPositionX(int x)
{
	base->area.x = x;
	base->tw_area.x = x;
	base->title_area.x = x;
	base->title_top.x = x;
	if (base->parent)
	{
		base->tw_area.x=base->area.x+base->parent->tw_area.x;
	}
}

void WinBaseWrapper::setComponentPositionY(int y)
{
	base->area.y = y;
	base->tw_area.y = y;
	base->title_area.y = y-17;
	base->title_top.y = y-17;
	if (base->parent)
	{
		base->tw_area.y=base->area.y+base->parent->tw_area.y;
	}
}
