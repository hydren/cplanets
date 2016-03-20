/*
 * planetarium.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef PLANETARIUM_HPP_
#define PLANETARIUM_HPP_

#include "util.hpp"
#include "physics/physics2d.hpp"
#include "sdl-widgets/sdl-widgets.h"

struct Planetarium extends WinBase
{
	SDL_Surface* surface;
	Physics2D* physics;
	Vector2D* viewportPosition;
	SDL_Color bgColor;

	//rendering parameters
	double zoom, minimumBodyRenderingRadius;
	int strokeSizeNormal, strokeSizeFocused;

	Planetarium(WinBase* parentWidget, Rect rect, Id _id=0);
	virtual ~Planetarium();
	void draw();

	Vector2D getTransposed(const Vector2D& position) const;
};



#endif /* PLANETARIUM_HPP_ */
