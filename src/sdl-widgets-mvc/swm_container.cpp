/*
 * swm_container.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "swm_container.hpp"

#include "futil/futil.hpp"

#include "swm_component.hpp"

SDL_GUIContainer::SDL_GUIContainer(int x, int y, int width, int height, SDL_GUIContainer* parent)
: SDL_GUIComponent(x, y, width, height, parent)
{}

SDL_GUIContainer::SDL_GUIContainer(float rel_x, float rel_y, int width, int height, SDL_GUIContainer* parent)
: SDL_GUIComponent(rel_x, rel_y, width, height, parent)
{}

SDL_GUIContainer::~SDL_GUIContainer()
{}

void SDL_GUIContainer::add(SDL_GUIComponent* component)
{
	childs.push_back(component);
}

void SDL_GUIContainer::remove(SDL_GUIComponent* component)
{
	childs.push_back(component);
}

void SDL_GUIContainer::paint() const
{
	paintComponents();
}

void SDL_GUIContainer::paintComponents() const
{
	const_foreach(const SDL_GUIComponent*, child, std::list<SDL_GUIComponent*>, childs)
		child->paintComponent();
}

