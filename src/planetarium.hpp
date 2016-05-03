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

#include "futil/futil.hpp"
#include "physics/physics2d.hpp"

struct Planetarium extends BgrWin, Physics2D::CollisionListener
{
	static const int USER_EVENT_ID__REDRAW_COMPONENT = 192; /// SDL_widgets user event ID hinting that it should be redrawed. Catch such events with a 'handle_uev' function and call 'doRefresh()'.
	static const double BODY_CREATION_DIAMETER_FACTOR = 32.0; //refinement factor

	static const unsigned DEFAULT_VIEWPORT_TRANSLATE_RATE = 8;
	static const double DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE = 0.1;
	static const double DEFAULT_BODY_CREATION_DIAMETER_RATIO = 1.0;
	static const double DEFAULT_BODY_CREATION_DENSITY = 1.0;
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
	bool tryAA;

	//interactive parameters
	unsigned viewportTranlationRateValue; //the intensity of translation when translating
	double viewportZoomChangeRateValue; //the intensity of zoom change when zooming
	Vector2D currentViewportTranlationRate; //non-zero when translating
	double currentViewportZoomChangeRate; //non-zero when zooming
	double bodyCreationDiameterRatio; //the default diameter of bodies, proportional to the view size. Zooming affects the diameter of newly created bodies.
	double bodyCreationDensity; //The default density of newly created bodies. Thus, zooming affects the mass of newly created bodies.

	Planetarium(WinBase* parentWidget, Rect rect, Id _id=0);
	virtual ~Planetarium();
	void draw();

	Vector2D getTransposed(const Vector2D& position) const;

	/** If run is true (default), the physics thread is started/resumed. Otherwise the thread is put to sleep. */
	void setRunning(bool run=true);

	/** It's not recommended to call this directly. */
	void doRefresh();

	//synchronized methods

	/** Assign a new random color to every body on the current universe (the safe way) */
	void recolorAllBodies();

	/** Adds a custom body (the safe way) */
	void addCustomBody(Body2D* body, SDL_Color* color);

	/** Returns a list of bodies on planetarium (the safe way). Changes on it does not reflect on the planetarium. */
	std::vector<Body2D> getBodies() const;


	// :::::::::::::::::::::: Inner classes ::::::::::::::::::::::::::::::::

	/** A struct to be subclassed to be able to listen to interesting universe events. */
	struct UniverseEventListener
	{
		virtual ~UniverseEventListener() {}
		virtual void onBodyCollision(std::vector<Body2D>& collidingList, Body2D& resultingMerger) abstract;
		virtual void onBodyCreation(Body2D& createdBody) abstract;
	};

	void addUniverseEventListener(UniverseEventListener* listener);
	void removeUniverseEventListener(UniverseEventListener* listener);

	/** A struct to record orbits. */
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


	/** Informs about visual body creation mode state. default is IDLE */
	enum BodyCreationState { IDLE, POSITION_SELECTION, VELOCITY_SELECTION } bodyCreationState;

	//================================================================================================================================================
	protected:
	struct Physics2DEventsManager; // helper struct to buffer collision events
	Physics2DEventsManager* physicsEventsManager;

	bool isUpdating;
	SDL_Thread* threadPhysics, *threadViewUpdate;
	SDL_mutex* physicsAccessMutex;
	std::vector<UniverseEventListener*> registeredBodyCollisionListeners;
	Vector2D bodyCreationPosition, bodyCreationVelocity;
	double bodyCreationDiameter;
	Uint32 lastMouseLeftButtonDown;

	void performPhysics(); //updates physics
	void updateView(); //updates view
	void onCollision(std::vector<Body2D*>& collidingList, Body2D& resultingMerger); //overrides Physics2D::CollisionListener

	static void onMouseDown(BgrWin*,int x,int y,int but);
	static void onMouseUp(BgrWin*,int x,int y,int but);

	static int threadFunctionPhysics(void* arg); //thread function
	static int threadFunctionPlanetariumUpdate(void* arg); //thread function
};



#endif /* PLANETARIUM_HPP_ */
