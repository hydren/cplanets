/*
 * flow_layout.cpp
 *
 *  Created on: 28 de mar de 2016
 *      Author: carlosfaruolo
 */

#include "flow_layout.hpp"

#include <stdexcept>
#include <algorithm>

#include "futil/futil.hpp"

using std::vector;

CPlanetsGUI::FlowLayout::FlowLayout(int x, int y)
{
	this->position.x = x;
	this->position.y = y;
}

CPlanetsGUI::FlowLayout::FlowLayout(Point& p)
{
	this->position = p;
}

void CPlanetsGUI::FlowLayout::addComponent(WinBase* component, int index)
{
	if(index < 0)
		this->components.push_back(component);
	else
		this->components.insert(components.begin()+index, component);
}

void CPlanetsGUI::FlowLayout::addComponent(WinBase& component, int index)
{
	CPlanetsGUI::FlowLayout::addComponent(&component, index);
}

void CPlanetsGUI::FlowLayout::removeComponentAt(unsigned index)
{
	if(index > this->components.size() - 1)
		throw std::out_of_range("out of range: " + index);
	this->components.erase(components.begin() + index);
}

void CPlanetsGUI::FlowLayout::removeComponent(WinBase& component)
{
	CPlanetsGUI::FlowLayout::removeComponent(&component);
}

void CPlanetsGUI::FlowLayout::removeComponent(WinBase* component)
{
	int index = Collections::indexOf(this->components, component);
	if(index < 0) return;
	CPlanetsGUI::FlowLayout::removeComponentAt(index);
}

void CPlanetsGUI::FlowLayout::pack()
{
	Point prevPosition = position;
	foreach(WinBase*, component, vector<WinBase*>, this->components)
	{
		CPlanetsGUI::setComponentPosition(component, prevPosition);
		prevPosition.x = component->area.x + WIDGETS_SPACING + component->tw_area.w;
	}
}
