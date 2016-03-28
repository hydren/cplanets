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

void CPlanetsGUI::FlowLayoutPanel::removeComponentAt(int index)
{
	if(index < 0 || index > this->components.size() - 1)
		throw std::out_of_range("out of range: " + index);
	this->components.erase(components.begin() + index);
}

void CPlanetsGUI::FlowLayoutPanel::removeComponent(WinBase& component)
{
	CPlanetsGUI::FlowLayoutPanel::removeComponent(&component);
}

void CPlanetsGUI::FlowLayoutPanel::removeComponent(WinBase* component)
{
	int i = 0;
	foreach(WinBase*, e, std::vector<WinBase*>, this->components)
	{
		if(e == component)
			break;
		i++;
	}
	if(i == this->components.size()) return; // has not found because 'i++' was done 'components.size()' times.
	CPlanetsGUI::FlowLayoutPanel::removeComponentAt(i);
}

void CPlanetsGUI::FlowLayoutPanel::pack()
{
	//ToDo arrange each component's tw_area side-by-side, horizontally.
}
