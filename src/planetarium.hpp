/*
 * planetarium.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef PLANETARIUM_HPP_
#define PLANETARIUM_HPP_

#include <map>

#include "SDL_widgets/SDL_widgets.h"

#include "util.hpp"
#include "physics/physics2d.hpp"

struct Planetarium extends WinBase
{
	struct UniverseEventListener;
	static const unsigned DEFAULT_VIEWPORT_TRANSLATE_RATE = 8;
	static const double DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE = 0.1;
	static const unsigned DEFAULT_STROKE_SIZE_NORMAL = 1, DEFAULT_STROKE_SIZE_FOCUSED = 2;
	static const unsigned DEFAULT_SLEEPING_TIME = 25;
	static const short DEFAULT_FPS = 60;

	//model
	Physics2D* physics;

	//widget parameters
	bool running;
	long sleepingTime;
	short fps;
	SDL_Color bgColor;
	int strokeSizeNormal, strokeSizeFocused;
	bool isViewportTranslationRateProportionalToZoom;

	//rendering parameters
	Vector2D viewportPosition;
	double viewportZoom, minimumBodyRenderingRadius;

	//interactive parameters
	unsigned viewportTranlationRateValue; //the intensity of translation when translating
	double viewportZoomChangeRateValue; //the intensity of zoom change when zooming
	Vector2D currentViewportTranlationRate; //non-zero when translating
	double currentViewportZoomChangeRate; //non-zero when zooming

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

	struct OrbitTracer
	{
		enum OrbitTraceStyle { POINT, LINEAR, SPLINE } style;
		bool isActive;
		unsigned traceLength;
		std::map<Body2D*, futil::iterable_queue<Vector2D> > traces;

		OrbitTracer();

		void record(Body2D* body, Vector2D& position);
		futil::iterable_queue<Vector2D> getTrace(Body2D* body);

	} orbitTracer;

	enum BodyCreationState{
		IDLE, POSITION_SELECTION, VELOCITY_SELECTION
	} bodyCreationState; //default is IDLE

	/** A struct to notify observers of collision between bodies.
	 * XXX REMEBER TO UNREGISTER AN UNUSED LISTENER */
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
	Vector2D bodyCreationPosition, bodyCreationVelocity;

	friend void bodyCollisionCallback(std::vector<Body2D*>& collidingList, Body2D& resultingMerger);
};



#endif /* PLANETARIUM_HPP_ */
