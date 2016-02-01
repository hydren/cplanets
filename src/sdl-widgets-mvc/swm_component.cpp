/*
 * swm_component.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "swm_component.hpp"

#include "sdl-widgets/sdl-widgets.h"

#include "swm_container.hpp"

#define X 0
#define Y 1

SDL_GUIComponent::SDL_GUIComponent(int x, int y, int width, int height, SDL_GUIContainer* parent)
: absolutePosition(x, y), width(width), height(height), visible(true), parent(parent)
{
	relativePosition[X] = -1;
	relativePosition[Y] = -1;
}

SDL_GUIComponent::SDL_GUIComponent(float relative_x, float relative_y, int width, int height, SDL_GUIContainer* parent)
: absolutePosition(-1, -1), width(width), height(height), visible(true), parent(parent)
{
	relativePosition[X] = relative_x;
	relativePosition[Y] = relative_y;
}

SDL_GUIComponent::~SDL_GUIComponent()
{
	parent->remove(this);
}

void SDL_GUIComponent::paint() const
{
//	SDL_FillRect()
}

int SDL_GUIComponent::getWidth() const
{
	return width;
}

int SDL_GUIComponent::getHeight() const
{
	return height;
}

int SDL_GUIComponent::getX() const
{
	return absolutePosition.x;
}

int SDL_GUIComponent::getY() const
{
	return absolutePosition.y;
}

SDL_Rect SDL_GUIComponent::getBounds() const
{
	return Rect(absolutePosition.x, absolutePosition.y, width, height);
}

int SDL_GUIComponent::getCenterX() const
{
	return (getX() + getWidth())/2;
}

int SDL_GUIComponent::getCenterY() const
{
	return (getY() + getHeight())/2;
}

float SDL_GUIComponent::getRelativeX() const
{
	return relativePosition[X];
}

float SDL_GUIComponent::getRelativeY() const
{
	return relativePosition[Y];
}

float* SDL_GUIComponent::getRelativePosition() const
{
	float* pos = new float[2];
	pos[X] = relativePosition[X];
	pos[Y] = relativePosition[Y];
	return pos;
}

void SDL_GUIComponent::paintComponent() const
{
	if(visible) paint();
}
