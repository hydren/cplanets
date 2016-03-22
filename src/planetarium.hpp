/*
 * planetarium.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef PLANETARIUM_HPP_
#define PLANETARIUM_HPP_

#include <pthread.h>

#include "SDL_widgets/SDL_widgets.h"

#include "util.hpp"
#include "physics/physics2d.hpp"

struct Planetarium extends WinBase
{
	SDL_Surface* surface;
	Physics2D* physics;
	Vector2D* viewportPosition;
	SDL_Color bgColor;

	//rendering parameters
	double zoom, minimumBodyRenderingRadius;
	int strokeSizeNormal, strokeSizeFocused;
	bool running;
	long sleepingTime;

	Planetarium(WinBase* parentWidget, Rect rect, Id _id=0);
	virtual ~Planetarium();
	void draw();

	Vector2D getTransposed(const Vector2D& position) const;

	void setRunning(bool run=true);
	void performPhysics();

	private:
	pthread_t physicsThread;
};



#endif /* PLANETARIUM_HPP_ */
