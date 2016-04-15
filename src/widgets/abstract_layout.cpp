/*
 * abstract_layout.cpp
 *
 *  Created on: 15 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "abstract_layout.hpp"
#include <typeinfo>

using std::vector;

CPlanetsGUI::Layout::WinBaseWrapper::WinBaseWrapper(WinBase* winBase)
: base(winBase)
{}

CPlanetsGUI::Layout::WinBaseWrapper::~WinBaseWrapper()
{}

Point CPlanetsGUI::Layout::WinBaseWrapper::getPosition()
{
	return base->area;
}

void CPlanetsGUI::Layout::WinBaseWrapper::setPosition(Point position)
{
	setX(position.x); setY(position.y);
}

void CPlanetsGUI::Layout::WinBaseWrapper::setPosition(int x, int y)
{
	setX(x); setY(y);
}

void CPlanetsGUI::Layout::WinBaseWrapper::setX(int x)
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

void CPlanetsGUI::Layout::WinBaseWrapper::setY(int y)
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

Rect CPlanetsGUI::Layout::WinBaseWrapper::getSize()
{
	return base->tw_area;
}

void CPlanetsGUI::Layout::WinBaseWrapper::setSize(Rect size)
{
	base->widen(size.w - base->tw_area.w, size.h - base->tw_area.h);
}

bool CPlanetsGUI::Layout::WinBaseWrapper::isStretched()
{
	return false;
}

bool CPlanetsGUI::Layout::WinBaseWrapper::operator==(const Element& someElement)
{
	if(typeid(someElement) != typeid(WinBaseWrapper))
		return false;

	else if(this->base != ((const WinBaseWrapper&) someElement).base)
		return false;

	else return true;
}

CPlanetsGUI::Layout::Layout(int x, int y)
{
	this->position.x = x;
	this->position.y = y;
}

CPlanetsGUI::Layout::Layout(Point& p)
{
	this->position = p;
}

CPlanetsGUI::Layout::~Layout(){}

void CPlanetsGUI::Layout::addComponent(WinBase* component, int index)
{
	if(index < 0)
		this->components.push_back(component);
	else
		this->components.insert(components.begin()+index, component);
}

void CPlanetsGUI::Layout::addComponent(WinBase& component, int index)
{
	CPlanetsGUI::Layout::addComponent(&component, index);
}

void CPlanetsGUI::Layout::removeComponentAt(unsigned index)
{
	if(index > this->components.size() - 1)
		throw std::out_of_range("out of range: " + index);
	this->components.erase(components.begin() + index);
}

void CPlanetsGUI::Layout::removeComponent(WinBase& component)
{
	CPlanetsGUI::Layout::removeComponent(&component);
}

void CPlanetsGUI::Layout::removeComponent(WinBase* component)
{
	int index = Collections::indexOf(this->components, component);
	if(index < 0) return;
	CPlanetsGUI::Layout::removeComponentAt(index);
}



