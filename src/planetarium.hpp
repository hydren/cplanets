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
	struct UniverseEventListener;

	//model
	Physics2D* physics;

	//widget parameters
	bool running;
	long sleepingTime;
	SDL_Color bgColor;
	int strokeSizeNormal, strokeSizeFocused;

	//rendering parameters
	Vector2D viewportPosition;
	double viewportZoom, minimumBodyRenderingRadius;

	//interactive parameters
	Vector2D currentViewportTranlationRate;
	double currentViewportZoomChangeRate;

	Planetarium(WinBase* parentWidget, Rect rect, Id _id=0);
	virtual ~Planetarium();
	void draw();

	Vector2D getTransposed(const Vector2D& position) const;

	/** If run is true (default), the physics thread is started/resumed. Otherwise the thread is put to sleep. */
	void setRunning(bool run=true);

	//--------------- \/ \/ SYNCHRONIZED METHODS \/ \/ -----------

	/** Assign a new random color to every body on the current universe (the safe way) */
	void recolorAllBodies();

	// Adds a custom body (the safe way)
	void addCustomBody(Body2D* body, SDL_Color* color);

	// Returns a list of bodies on planetarium (the safe way). Changes on it does not reflect on the planetarium.
	std::vector<Body2D> getBodies() const;

	//--------------- /\ /\ SYNCHRONIZED METHODS /\ /\ -----------

	void addUniverseEventListener(UniverseEventListener* listener);
	void removeUniverseEventListener(UniverseEventListener* listener);

	// Its not recommended to call this directly
	void performPhysics();

	// Its not recommended to call this directly
	void updateView();

	/** A struct to notify observers of collision between bodies.
	 * XXX REMEBER TO UNREGISTER AN UNUSED LISTENER*/
	struct UniverseEventListener
	{
		virtual ~UniverseEventListener() {}
		virtual void onBodyCollision(std::vector<Body2D>& collidingList, Body2D& resultingMerger) abstract;
		virtual void onBodyCreation(Body2D& createdBody) abstract;
	};

	//================================================================================================================================================
	private:
	SDL_Thread* threadPhysics, *threadViewUpdate;
	SDL_mutex* physicsAccessMutex;
	std::vector<UniverseEventListener*> registeredBodyCollisionListeners;
	friend void bodyCollisionCallback(std::vector<Body2D*>& collidingList, Body2D& resultingMerger);

	struct PlanetariumUserObject
	{
		PlanetariumUserObject(SDL_Color* color);
		SDL_Color* color;
	};
};



#endif /* PLANETARIUM_HPP_ */
