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

using SDL_util::FlowLayout;

FlowLayout::FlowLayout(int x, int y, Rect max)
: SDL_util::Layout(x, y), maxSize(max), spacing_h(DEFAULT_SPACING), spacing_v(DEFAULT_SPACING), alignment(BEFORE)
{}

FlowLayout::FlowLayout(Point p)
: SDL_util::Layout(p), maxSize(Rect(-1,-1,0,0)), spacing_h(DEFAULT_SPACING), spacing_v(DEFAULT_SPACING), alignment(BEFORE)
{}

FlowLayout::~FlowLayout()
{}

void FlowLayout::pack()
{
	int currentStretchedSpacerSize = this->needsStretching()? this->computeFreeSpaceOnLayout() / this->getStretchingElementsCount() : -1 ;
	Point prevPosition = position;
	foreach(Element*, component, vector<Element*>, this->components)
	{
		prevPosition.x += component->offset.x; //adds the offset to the position (to consume the space)
		Point actualPosition = prevPosition;
		actualPosition.y += component->offset.y + computeAlignment(component, actualPosition.y);
		component->setPosition(actualPosition); //sends a copy rect with the y axis offset'd by the component's y-offset
		prevPosition.x += spacing_h + (component->isStretched()? currentStretchedSpacerSize : component->getSize().w); //adds the current component width and global spacing
	}
}

bool FlowLayout::needsStretching() const
{
	const_foreach(const Element*, component, vector<Element*>, this->components)
		if(component->isStretched())
			return true;

	return false;
}

unsigned FlowLayout::computeFreeSpaceOnLayout() const
{
	int space = this->maxSize.w == 0? SDL_GetVideoInfo()->current_w - this->position.x : this->maxSize.w;
	const_foreach(const Element*, component, vector<Element*>, this->components)
	{
		if(not component->isStretched())
			space -= component->getSize().w; //subtract each non-stretched component size
	}

	space -= spacing_h * (1 + this->components.size()); //subtract each spacing between components

	return space > 0 ? space : 0;
}

int FlowLayout::computeAlignment(Element* e, int pos) const
{
	if(this->alignment == BEFORE) return 0;

	int space = this->maxSize.h == 0? SDL_GetVideoInfo()->current_h - this->position.y : this->maxSize.h;
	space -= e->getSize().h;

	if(this->alignment == AFTER)
		return space;

	else if(this->alignment == MIDDLE)
		return space * 0.5;

	//be careful to not fall into this one
	return 0;
}

unsigned FlowLayout::getStretchingElementsCount() const
{
	unsigned count = 0;
	const_foreach(const Element*, component, vector<Element*>, this->components)
		if(component->isStretched())
			count++;

	return count;
}
