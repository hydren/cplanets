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
using std::pair;
using std::set;

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

CPlanetsGUI::FlowLayout::FlowLayout(int x, int y)
: CPlanetsGUI::Layout(x, y)
{}

CPlanetsGUI::FlowLayout::FlowLayout(Point& p)
: CPlanetsGUI::Layout(p)
{}

CPlanetsGUI::FlowLayout::~FlowLayout()
{}

void CPlanetsGUI::FlowLayout::pack()
{
	int currentStretchedSpacerSize = this->needsSpacerStretching()? this->computeFreeSpaceOnLayout() / this->spacersSet.size() : -1 ;
	int i = 0;
	Point prevPosition = position;
	foreach(WinBase*, component, vector<WinBase*>, this->components)
	{
		int spacer = getSpacerSize(i++);
		prevPosition.x += spacer == -1? currentStretchedSpacerSize : spacer; //adds the spacer size as offset
		CPlanetsGUI::setComponentPosition(component, prevPosition);
		prevPosition.x = component->area.x + WIDGETS_SPACING + component->tw_area.w; //adds the current component width and global spacing
	}
}

void CPlanetsGUI::FlowLayout::addSpacer(int size, int index)
{
	if(index < 0)
		index = this->components.size();

	this->spacersSet.insert(pair<int, int>(index, size));
}

void CPlanetsGUI::FlowLayout::removeSpacer(int index)
{
	typedef pair<int, int> PairOfTwoInts;
	foreach(const PairOfTwoInts&, p, set<PairOfTwoInts>, this->spacersSet)
	{
		//removes all spacers on 'afterIndex'
		if(p.first == index)
			this->spacersSet.erase(this->spacersSet.find(p));
	}
}

int CPlanetsGUI::FlowLayout::getSpacerSize(int index)
{
	typedef pair<int, int> PairOfTwoInts;
	foreach(const PairOfTwoInts&, p, set<PairOfTwoInts>, this->spacersSet)
	{
		if(p.first == index)
			return p.second;
	}
	return 0;
}

bool CPlanetsGUI::FlowLayout::needsSpacerStretching() const
{
	if(this->spacersSet.size() < 1) //if there are no spacers, no stretch is needed
		return false;

	typedef pair<int, int> PairOfTwoInts;
	foreach(const PairOfTwoInts&, p, set<PairOfTwoInts>, this->spacersSet)
		if(p.second == -1)
			return true;

	return false;
}

unsigned CPlanetsGUI::FlowLayout::computeFreeSpaceOnLayout() const
{
	int space = SDL_GetVideoInfo()->current_w - this->position.x;
	const_foreach(const WinBase*, component, vector<WinBase*>, this->components)
		space -= component->tw_area.w; //subtract each component width

	space -= WIDGETS_SPACING * (1 + this->components.size()); //subtract each spacing between components

	return space > 0 ? space : 0;
}
