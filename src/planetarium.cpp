/*
 * planetarium.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium.hpp"

Planetarium::Planetarium(WinBase* parentWidget, Rect rect, Id _id)
: WinBase(parentWidget, 0, rect.x, rect.y, rect.w, rect.h, 0, _id),
  surface(SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0)),
  physics(), viewportPosition(new Vector2D()), bgColor(SDL_Color())
{
	if(this->surface == null)
		throw_exception("Failed to create Planetarium surface: %s", SDL_GetError());
}

Planetarium::~Planetarium()
{
	SDL_FreeSurface(this->surface);
	delete this->viewportPosition;
}

void Planetarium::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, SDL_MapRGB(win->format, this->bgColor.r, this->bgColor.g, this->bgColor.b)); //clears
}


