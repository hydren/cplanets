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

CPlanetsGUI::FlowLayout::FlowLayout(int x, int y, int max)
: CPlanetsGUI::Layout(x, y), maxSize(max)
{}

CPlanetsGUI::FlowLayout::FlowLayout(Point p)
: CPlanetsGUI::Layout(p), maxSize(0)
{}

CPlanetsGUI::FlowLayout::~FlowLayout()
{}

void CPlanetsGUI::FlowLayout::pack()
{
	int currentStretchedSpacerSize = this->needsStretching()? this->computeFreeSpaceOnLayout() / this->getStretchingElementsCount() : -1 ;
	Point prevPosition = position;
	foreach(Element*, component, vector<Element*>, this->components)
	{
		prevPosition.x += component->offset.x; //adds the offset to the position (to consume the space)
		Point actualPosition = prevPosition;
		actualPosition.y += component->offset.y;
		component->setPosition(actualPosition); //sends a copy rect with the y axis offset'd by the component's y-offset
		prevPosition.x += WIDGETS_SPACING + (component->isStretched()? currentStretchedSpacerSize : component->getSize().w); //adds the current component width and global spacing
	}
}

bool CPlanetsGUI::FlowLayout::needsStretching() const
{
	const_foreach(const Element*, component, vector<Element*>, this->components)
		if(component->isStretched())
			return true;

	return false;
}

unsigned CPlanetsGUI::FlowLayout::computeFreeSpaceOnLayout() const
{
	int space = this->maxSize == 0? SDL_GetVideoInfo()->current_w - this->position.x : this->maxSize;
	const_foreach(const Element*, component, vector<Element*>, this->components)
	{
		if(not component->isStretched())
			space -= component->getSize().w; //subtract each non-stretched component size
	}

	space -= WIDGETS_SPACING * (1 + this->components.size()); //subtract each spacing between components

	return space > 0 ? space : 0;
}

unsigned CPlanetsGUI::FlowLayout::getStretchingElementsCount() const
{
	unsigned count = 0;
	const_foreach(const Element*, component, vector<Element*>, this->components)
		if(component->isStretched())
			count++;

	return count;
}
