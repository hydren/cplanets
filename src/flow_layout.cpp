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

CPlanetsGUI::FlowLayoutPanel::FlowLayoutPanel(int x, int y)
{
	this->position.x = x;
	this->position.y = y;
}

CPlanetsGUI::FlowLayoutPanel::FlowLayoutPanel(Point& p)
{
	this->position = p;
}

void CPlanetsGUI::FlowLayoutPanel::addComponent(WinBase* component, int index)
{
	if(index < 0)
		this->components.push_back(component);
	else
		this->components.insert(components.begin()+index, component);
}

void CPlanetsGUI::FlowLayoutPanel::addComponent(WinBase& component, int index)
{
	CPlanetsGUI::FlowLayoutPanel::addComponent(&component, index);
}

void CPlanetsGUI::FlowLayoutPanel::removeComponentAt(unsigned index)
{
	if(index > this->components.size() - 1)
		throw std::out_of_range("out of range: " + index);
	this->components.erase(components.begin() + index);
}

void CPlanetsGUI::FlowLayoutPanel::removeComponent(WinBase& component)
{
	CPlanetsGUI::FlowLayoutPanel::removeComponent(&component);
}

void CPlanetsGUI::FlowLayoutPanel::removeComponent(WinBase* component)
{
	int index = Collections::indexOf(this->components, component);
	if(index < 0) return;
	CPlanetsGUI::FlowLayoutPanel::removeComponentAt(index);
}

void CPlanetsGUI::FlowLayoutPanel::pack()
{
	Point prevPosition = position;
	foreach(WinBase*, component, vector<WinBase*>, this->components)
	{
		CPlanetsGUI::setComponentPosition(component, prevPosition);
		prevPosition.x = component->area.x + WIDGETS_SPACING + component->tw_area.w;
	}
}
