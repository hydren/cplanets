/*
 * planetarium.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium.hpp"
#include "main_window.hpp"

using CPlanetsGUI::colorToInt;
using CPlanetsGUI::modifyColor;
using std::cout; using std::endl;

Planetarium::Planetarium(WinBase* parentWidget, Rect rect, Id _id)
: WinBase(parentWidget, 0, rect.x, rect.y, rect.w, rect.h, 0, _id),
  surface(SDL_CreateRGBSurface(SDL_SWSURFACE, rect.w, rect.h, 32, 0, 0, 0, 0)),
  physics(new Physics2D()), viewportPosition(new Vector2D()), bgColor(SDL_Color()),
  zoom(1.0), minimumBodyRenderingRadius(3.0),
  strokeSizeNormal(1), strokeSizeFocused(2)
{
	if(this->surface == null)
		throw_exception("Failed to create Planetarium surface: %s", SDL_GetError());

	modifyColor(bgColor, 0, 0, 0);
}

Planetarium::~Planetarium()
{
	SDL_FreeSurface(this->surface);
	delete this->viewportPosition;
}

void Planetarium::draw()
{
	this->init_gui();
	SDL_FillRect(this->surface, null, colorToInt(this->surface, bgColor)); //clears the screen

	//draw bodies
	foreach(Body2D&, body, std::list<Body2D>, this->physics->universe.bodies)
	{
		cout << "drawing body " << body.id << endl;
		double size = zoom*body.diameter;
		if(size < this->minimumBodyRenderingRadius) size = this->minimumBodyRenderingRadius;
		SDL_Color* bodyColor = (SDL_Color*) body.userObject;

		Vector2D v = this->getTransposed(body.position);

		cout << "(x, y) = " << v.x << ", " << v.y << endl;

		cout << "size: " << size << endl;

//		SDL_Rect tmpRect; tmpRect.x = x; tmpRect.y = y; tmpRect.h = size, tmpRect.w = size;
//		SDL_FillRect(this->surface, &tmpRect, colorToInt(*bodyColor));

		filledCircleColor(this->surface, v.x, v.y, round(size*0.5), colorToInt(null, *bodyColor, true));

		int borderColor = 0xffffffff;

//		if(focusedBodies.contains(body))
//		{
//			graphics2d.setStroke(focusedStroke);
//			borderColor = int value for orange
//		}

		circleColor(this->surface, v.x, v.y, round(size*0.5), borderColor);
	}

	SDL_BlitSurface(this->surface, null, this->win, null);
}

Vector2D Planetarium::getTransposed(const Vector2D& position) const
{
	cout << "viewport: " << (*(this->viewportPosition)).x << ", " << (*(this->viewportPosition)).y << endl;
	return position.difference(*(this->viewportPosition)).scale(zoom);
}


