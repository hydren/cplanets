/*
 * planetarium.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef PLANETARIUM_HPP_
#define PLANETARIUM_HPP_

#include "physics/physics2d.hpp"
#include "sdl-widgets/sdl-widgets.h"

struct Planetarium : public WinBase
{
	SDL_Surface* surface;
	Physics2D* physics;
	Vector2D* viewportPosition;
	SDL_Color bgColor;

	Planetarium(WinBase* parentWidget, Rect rect, Id _id=0);
	virtual ~Planetarium();
	void draw();
};



#endif /* PLANETARIUM_HPP_ */
