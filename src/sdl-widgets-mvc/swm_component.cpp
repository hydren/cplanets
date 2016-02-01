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
: absoluteBounds(Rect(x, y, width, height)), parent(parent)
{
	relativePosition[X] = -1;
	relativePosition[Y] = -1;
}

SDL_GUIComponent::SDL_GUIComponent(float relative_x, float relative_y, int width, int height, SDL_GUIContainer* parent)
: absoluteBounds(Rect(-1, -1, width, height)), parent(parent)
{
	relativePosition[X] = relative_x;
	relativePosition[Y] = relative_y;
}

SDL_GUIComponent::~SDL_GUIComponent()
{
	parent->remove(this);
}


