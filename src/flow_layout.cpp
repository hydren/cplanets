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
	//ToDo arrange each component's tw_area side-by-side, horizontally.
}
