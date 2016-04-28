/*
 * abstract_layout.cpp
 *
 *  Created on: 15 de abr de 2016
 *      Author: carlosfaruolo
 */

#include "abstract_layout.hpp"
#include <typeinfo>

using std::vector;

// WinBaseWrapper =================================================================

SDL_util::Layout::WinBaseWrapper::WinBaseWrapper(WinBase* winBase)
: base(winBase)
{}

SDL_util::Layout::WinBaseWrapper::~WinBaseWrapper()
{}

Point SDL_util::Layout::WinBaseWrapper::getPosition() const
{
	return base->area;
}

void SDL_util::Layout::WinBaseWrapper::setPosition(Point position)
{
	setWinBasePosition(this->base, position);
}

void SDL_util::Layout::WinBaseWrapper::setPosition(int x, int y)
{
	setX(x); setY(y);
}

void SDL_util::Layout::WinBaseWrapper::setX(int x)
{
	setWinBasePositionX(this->base, x);
}

void SDL_util::Layout::WinBaseWrapper::setY(int y)
{
	setWinBasePositionY(this->base, y);
}

Rect SDL_util::Layout::WinBaseWrapper::getSize() const
{
	return base->tw_area;
}

void SDL_util::Layout::WinBaseWrapper::setSize(Rect size)
{
	base->widen(size.w - base->tw_area.w, size.h - base->tw_area.h);
}

bool SDL_util::Layout::WinBaseWrapper::isStretched() const
{
	return false;
}

bool SDL_util::Layout::WinBaseWrapper::operator==(const Element& someElement) const
{
	if(typeid(someElement) != typeid(WinBaseWrapper))
		return false;

	else if(this->base != ((const WinBaseWrapper&) someElement).base)
		return false;

	else return true;
}

void SDL_util::Layout::WinBaseWrapper::setWinBasePosition(WinBase* wb, Point pos)
{
	setWinBasePositionX(wb, pos.x);
	setWinBasePositionY(wb, pos.y);
}

void SDL_util::Layout::WinBaseWrapper::setWinBasePositionX(WinBase* wb, int x)
{
	wb->area.x = x;
	wb->tw_area.x = x;
	wb->title_area.x = x;
	wb->title_top.x = x;
	if (wb->parent)
	{
		wb->tw_area.x=wb->area.x+wb->parent->tw_area.x;
	}
}

void SDL_util::Layout::WinBaseWrapper::setWinBasePositionY(WinBase* wb, int y)
{
	wb->area.y = y;
	wb->tw_area.y = y;
	wb->title_area.y = y-17;
	wb->title_top.y = y-17;
	if (wb->parent)
	{
		wb->tw_area.y=wb->area.y+wb->parent->tw_area.y;
	}
}

// Spacer ========================================================================================

SDL_util::Layout::Spacer::Spacer(Layout* layout, Rect size)
: bounds(layout->position.x, layout->position.y, size.w, size.h),
  stretched(size.w == 0 && size.h == 0)
{}

SDL_util::Layout::Spacer::Spacer(Rect bounds)
: bounds(bounds), stretched(bounds.w == 0 && bounds.h == 0)
{}

SDL_util::Layout::Spacer::~Spacer() {}

Point SDL_util::Layout::Spacer::getPosition() const
{
	return Point(this->bounds.x, this->bounds.y);
}

void SDL_util::Layout::Spacer::setPosition(Point position)
{
	this->bounds.x = position.x;
	this->bounds.y = position.y;
}

Rect SDL_util::Layout::Spacer::getSize() const
{
	return Rect(0, 0, this->bounds.w, this->bounds.h);
}

void SDL_util::Layout::Spacer::setSize(Rect size)
{
	this->bounds.w = size.w;
	this->bounds.h = size.h;
}

bool SDL_util::Layout::Spacer::isStretched() const
{
	return this->stretched;
}

bool SDL_util::Layout::Spacer::operator == (const Element& b) const
{
	return &b == this;
}

// Layout ========================================================================================

SDL_util::Layout::Layout(int x, int y)
{
	this->position.x = x;
	this->position.y = y;
}

SDL_util::Layout::Layout(Point& p)
{
	this->position = p;
}

SDL_util::Layout::~Layout(){}

void SDL_util::Layout::addComponent(WinBase& base, int index)
{
	addComponent(&base, index);
}

void SDL_util::Layout::addComponent(WinBase* base, int index)
{
	WinBaseWrapper* component = new WinBaseWrapper(base); //creater wrapper for winbase
	innerWrappers.push_back(component); //register the layout-created wrapper
	addComponent(reinterpret_cast<Element*>(component));
}

void SDL_util::Layout::addComponent(Element& component, int index)
{
	addComponent(&component, index);
}

void SDL_util::Layout::addComponent(Element* component, int index)
{
	if(index < 0)
		this->components.push_back(component);
	else
		this->components.insert(components.begin()+index, component);
}

/** Returns a pointer to the component at the specified index */
SDL_util::Layout::Element* SDL_util::Layout::getComponentAt(unsigned index)
{
	if(index > this->components.size() - 1)
		throw std::out_of_range("out of range: " + index);

	return this->components[index];
}

/** Returns a pointer to the wrapper component corresponding to the given WinBase */
SDL_util::Layout::WinBaseWrapper* SDL_util::Layout::getWrapperComponent(WinBase* base)
{
	for(unsigned i = 0; i < this->components.size(); i++)
	{
		WinBaseWrapper* wrapper = dynamic_cast<WinBaseWrapper*>(this->components.at(i));
		if(wrapper != null && wrapper->base == base)
			return wrapper;
	}
	return null;
}

void SDL_util::Layout::removeComponentAt(unsigned index)
{
	if(index > this->components.size() - 1)
		throw std::out_of_range("out of range: " + index);

	//if there is a pointer to the element on innerWrappers, then it is a WinBaseWrapper created by this layout
	for(int w = Collections::indexOf(this->innerWrappers, reinterpret_cast<WinBaseWrapper*>(this->components[index])); w != -1; w = -1)
	{
		delete this->innerWrappers[w]; //delete the wrapper
		this->innerWrappers.erase(this->innerWrappers.begin() + w); //remove the pointer
	}

	//remove the pointer
	this->components.erase(components.begin() + index);
}

void SDL_util::Layout::removeComponent(WinBase& component)
{
	SDL_util::Layout::removeComponent(&component);
}

void SDL_util::Layout::removeComponent(WinBase* component)
{
	for(unsigned i = 0; i < this->components.size(); i++)
	{
		WinBaseWrapper* wrapper = dynamic_cast<WinBaseWrapper*>(this->components.at(i));
		if(wrapper != null && wrapper->base == component)
		{
			SDL_util::Layout::removeComponentAt(i);
			break;
		}
	}
}

void SDL_util::Layout::removeComponent(Element& component)
{
	SDL_util::Layout::removeComponent(&component);
}

void SDL_util::Layout::removeComponent(Element* component)
{
	int index = Collections::indexOf(this->components, component);
	if(index < 0) return;
	SDL_util::Layout::removeComponentAt(index);
}

