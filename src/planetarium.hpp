/*
 * planetarium.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef PLANETARIUM_HPP_
#define PLANETARIUM_HPP_

#include "SDL_widgets/SDL_widgets.h"

#include "util.hpp"
#include "physics/physics2d.hpp"

struct Planetarium extends WinBase
{
	struct BodyCollisionListener;
	Physics2D* physics;
	Vector2D viewportPosition;
	SDL_Color bgColor;

	//rendering parameters
	double zoom, minimumBodyRenderingRadius;
	int strokeSizeNormal, strokeSizeFocused;
	bool running;
	long sleepingTime;

	//interactive parameters
	Vector2D currentViewportTranlationRate;

	Planetarium(WinBase* parentWidget, Rect rect, Id _id=0);
	virtual ~Planetarium();
	void draw();

	Vector2D getTransposed(const Vector2D& position) const;

	/** If run is true (default), the physics thread is started/resumed. Otherwise the thread is put to sleep. */
	void setRunning(bool run=true);

	/** Assign a new random color to every body on the current universe */
	void recolorAllBodies();

	void addCustomBody(Body2D* body, SDL_Color* color);

	void addCollisionListener(BodyCollisionListener* listener);
	void removeCollisionListener(BodyCollisionListener* listener);

	void performPhysics();
	void updateView();

	/** A struct to notify observers of collision between bodies.
	 * XXX REMEBER TO UNREGISTER AN UNUSED LISTENER*/
	struct BodyCollisionListener
	{
		virtual ~BodyCollisionListener() {}
		virtual void onBodyCollision(std::vector<Body2D*>& collidingList, Body2D& resultingMerger) abstract;
	};

	//================================================================================================================================================
	private:
	SDL_Thread* threadPhysics, *threadViewUpdate;
	friend void bodyCollisionCallback(std::vector<Body2D*>& collidingList, Body2D& resultingMerger);

	struct PlanetariumUserObject
	{
		PlanetariumUserObject(SDL_Color* color);
		SDL_Color* color;
	};
};



#endif /* PLANETARIUM_HPP_ */
