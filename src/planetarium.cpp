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
  physics(new Physics2D()), viewportPosition(new Vector2D()), bgColor(SDL_Color()),
  zoom(1.0), minimumBodyRenderingRadius(3.0),
  strokeSizeNormal(1), strokeSizeFocused(2)
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
	SDL_FillRect(this->win, null, SDL_MapRGB(win->format, this->bgColor.r, this->bgColor.g, this->bgColor.b)); //clears the screen

	//draw bodies
	foreach(Body2D&, body, std::list<Body2D>, this->physics->universe.bodies)
	{
		double size = zoom*body.diameter;
		if(size < this->minimumBodyRenderingRadius) size = this->minimumBodyRenderingRadius;

		Vector2D v = this->getTransposed(body.position);
		int x = (int) round(v.x - size/2 );
		int y = (int) round(v.y - size/2 );

		filledCircleColor(this->surface, x, y, round(size*0.5), body.color);

//		if(focusedBodies.contains(body))
//		{
//			graphics2d.setStroke(focusedStroke);
//			graphics2d.setColor( Color.ORANGE );
//		}
//		else
//			graphics2d.setColor( Color.WHITE );

		circleColor(this->surface, x, y, this->strokeSizeNormal, 0xffffff);
	}
}

Vector2D Planetarium::getTransposed(const Vector2D& position) const
{
	return position.difference(*(this->viewportPosition)).scale(zoom);
}


