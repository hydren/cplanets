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

#include "main_window.hpp"

using std::vector;
using std::pair;
using std::set;

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
	int currentStretchedSpacerSize = this->needsSpacerStretching()? this->computeFreeSpaceOnLayout() / this->getStretchedSpacersCount() : -1 ;
	int i = 0;
	Point prevPosition = position;
	foreach(Element*, component, vector<Element*>, this->components)
	{
		int spacer = getSpacerSize(i++);
		prevPosition.x += spacer == -1? currentStretchedSpacerSize : spacer; //adds the spacer size as offset
		component->setPosition(prevPosition);
		prevPosition.x = component->getPosition().x + WIDGETS_SPACING + component->getSize().w; //adds the current component width and global spacing
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
	const_foreach(const Element*, component, vector<Element*>, this->components)
		space -= const_cast<Element*>(component)->getSize().w; //subtract each component size

	space -= WIDGETS_SPACING * (1 + this->components.size()); //subtract each spacing between components

	typedef pair<int, int> PairOfTwoInts;
	foreach(const PairOfTwoInts&, p, set<PairOfTwoInts>, this->spacersSet) //subtract each fixed-size spacer size
		if(p.second != -1)
			space -= p.second;

	return space > 0 ? space : 0;
}

unsigned CPlanetsGUI::FlowLayout::getStretchedSpacersCount() const
{
	unsigned count = 0;
	typedef pair<int, int> PairOfTwoInts;
	foreach(const PairOfTwoInts&, p, set<PairOfTwoInts>, this->spacersSet)
		if(p.second == -1)
			count++;
	return count;
}
