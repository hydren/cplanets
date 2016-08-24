/*
 * planetarium.hpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#ifndef PLANETARIUM_HPP_
#define PLANETARIUM_HPP_

#include <SDL/SDL.h>
#include <map>

#include "physics/physics2d.hpp"
#include "futil/futil.hpp"

struct Planetarium extends Physics2D::Listener
{
	static const double BODY_CREATION_DIAMETER_FACTOR = 32.0; //refinement factor

	static const unsigned DEFAULT_VIEWPORT_TRANSLATE_RATE = 8;
	static const double DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE = 0.1;
	static const double DEFAULT_BODY_CREATION_DIAMETER_RATIO = 1.0;
	static const double DEFAULT_BODY_CREATION_DENSITY = 1.0;
	static const unsigned DEFAULT_STROKE_SIZE_NORMAL = 1, DEFAULT_STROKE_SIZE_FOCUSED = 2;
	static const double DEFAULT_MINIMUM_BODY_RENDERING_RADIUS = 3.0;
	static const unsigned DEFAULT_SLEEPING_TIME = 25;
	static const short DEFAULT_FPS = 60;
	static const long DEFAULT_DISPLAY_PERIOD = 30, DEFAULT_ITERATIONS_PER_DISPLAY = 2;

	SDL_Surface* surf;
	SDL_Rect size, pos;
	bool isRedrawPending; /// advanced flag

	// auxiliar structure
	struct Body2DClone;

	//model
	Physics2D* physics;

	//simulation parameters
	bool running;
	long stepDelay;
	short fps;
	bool legacyControl;
	long displayPeriod, iterationsPerDisplay;

	//widget parameters
	SDL_Color bgColor, strokeColorNormal, strokeColorFocused;
	int strokeSizeNormal, strokeSizeFocused;
	bool isViewportTranslationRateProportionalToZoom;
	bool pauseOnSelection;

	//rendering parameters
	Vector2D viewportPosition;
	double viewportZoom, minimumBodyRenderingRadius;
	std::vector<Body2D*> focusedBodies;
	bool tryAA;

	//interactive parameters
	unsigned viewportTranlationRateValue; //the intensity of translation when translating
	double viewportZoomChangeRateValue; //the intensity of zoom change when zooming
	Vector2D currentViewportTranlationRate; //non-zero when translating
	double currentViewportZoomChangeRate; //non-zero when zooming
	double bodyCreationDiameterRatio; //the default diameter of bodies, proportional to the view size. Zooming affects the diameter of newly created bodies.
	double bodyCreationDensity; //The default density of newly created bodies. Thus, zooming affects the mass of newly created bodies.

	/** Creates a planetarium with a surface size specified by 'rect'. Optionally a pixel depth can be specified, but it's not recommended pass anything but the default. */
	Planetarium(SDL_Rect rect, Uint32 pixdepth=16);
	virtual ~Planetarium();

	/** Starts the threads responsible for physics and view update. */
	void start();

	/** The drawing method. Normally you should not need to call this directly. */
	void draw();

	/** Resizes the planetarium surface by the given amount. */
	void widen(int dx, int dy);

	/** Gets the given physics position in screen position. */
	Vector2D getTransposed(const Vector2D& position) const;

	/** Gets the given screen position in physics position. */
	Vector2D getAntiTransposed(const Vector2D& position) const;

	/** If run is true (default), the physics thread is started/resumed. Otherwise the thread is put to sleep. */
	void setRunning(bool run=true);

	/** Activates/Deactivates body creation mode; */
	void setBodyCreationMode(bool enable=true);

	/** Sets the given bodies as the focused/selected ones. 'bodyarr' should be an n-sized array of Body2D pointers. 'bodyarr' size is less than n, expect seg.faults. */
	void setFocusedBodies(Body2D*const* bodyarr, unsigned n);

	/** Sets the given bodies as the focused/selected ones. */
	void setFocusedBodies(const std::vector<Body2D*>& bodies);

	//synchronized methods

	/** Removes all focused bodies from the universe. By default, it also deletes these bodies. If 'alsoDelete' is false, then it wont delete these bodies.
	 *  Note that even if you prevent deletion of the focused bodies, you'll need to copy the vector of focused bodies beforehand, as this method also clears the 'focusedBodies' vector. */
	void removeFocusedBodies(bool alsoDelete = true);

	/** Assign a new random color to every body on the current universe (the safe way) */
	void recolorAllBodies();

	/** Adds (safely) a custom body. If no color is specified, a random color will be used. */
	void addCustomBody(Body2D* body, SDL_Color* color=null);

	/** Adds (safely) a custom body with the given parameters. If no color is specified, a random color will be used. */
	void addCustomBody(double mass, double diameter, const Vector2D& position, const Vector2D& velocity, SDL_Color* color=null);

	/** Adds a random body with resulting characteristics being, on average, the given parameters. If an area is specified, the resulting body will be positioned randomly within it.*/
	void addRandomBody(double avMass, double avDiameter, double avVelocity, const double area[4]=null);

	/** Removes 'body' from the universe, if it contains it. If 'alsoDelete' is true, calls the the destructor of 'body' after its removal. If the universe does't contain 'body', then 'body' remains intact. */
	void removeBody(Body2D* body, bool alsoDelete = false);

	/** Returns a list of bodies on planetarium (the safe way). Changes on it does not reflect on the planetarium. */
	std::vector<Body2DClone> getBodies() const;

	/** Safer way to replace the universe instance. */
	void setUniverse(const Universe2D& u);

	long double getTotalKineticEnergy() const; //synchronized version
	long double getTotalPotentialEnergy() const; //synchronized version
	unsigned getBodyCount() const; //synchronized version

	/** Optional dispatcher to perform updates on the resulting surface. */
	struct SurfaceUpdateDispatcher
	{
		/** Called when a surface needs to be updated. */
		virtual void onSurfaceUpdate() abstract;
		virtual ~SurfaceUpdateDispatcher(){}
	}*drawDispatcher;

	/** Action to be called when a mouse button is pressed */
	void onMouseButtonPressed(int mouseX, int mouseY, int mouseButtonNumber);

	/** Action to be called when a mouse button is released */
	void onMouseButtonReleased(int mouseX, int mouseY, int mouseButtonNumber);

	// :::::::::::::::::::::: Inner classes ::::::::::::::::::::::::::::::::

	/** A struct to be subclassed to be able to listen to interesting universe events. */
	struct UniverseEventListener
	{
		virtual ~UniverseEventListener() {}
		virtual void onBodyCollision(std::vector<Body2D>& collidingList, Body2D& resultingMerger) {}
		virtual void onBodyCreation(Body2D& createdBody) {}
		virtual void onBodyDeletion(Body2D* deletedBody) {} //not exactly safe to use the pointer after the call ends
		virtual void onBodyReFocus() {}
	};

	/// The listener manager. Add and remove listener with it.
	futil::ListenerManager<UniverseEventListener> listeners;

	/** A struct to record orbits. */
	struct OrbitTracer
	{
		enum OrbitTraceStyle { DOTTED, LINEAR, SPLINE } style;
		bool isActive;
		unsigned traceLength;
		std::map<Body2D*, futil::iterable_queue<Vector2D> > traces;

		OrbitTracer(Planetarium* p);

		void record(Body2D* body);
		futil::iterable_queue<Vector2D>& getTrace(Body2D* body);
		void clearTrace(const Body2D* body);

		void drawTrace(Body2D* body);
		void drawTrace(Body2DClone& body);

		protected:
		Planetarium* planetarium;

		void drawTrace(futil::iterable_queue<Vector2D>& trace, SDL_Color* color);

		void drawDotted(futil::iterable_queue<Vector2D>& trace, SDL_Color* color);
		void drawLinear(futil::iterable_queue<Vector2D>& trace, SDL_Color* color);

		void drawQuadricBezier(futil::iterable_queue<Vector2D>& trace, SDL_Color* color); //still not working properly
//		void drawCubicBezier(futil::iterable_queue<Vector2D>& trace, SDL_Color* color);  //not implemented

	} orbitTracer;


	/** Informs about visual body creation mode state. default is IDLE */
	enum BodyCreationState { IDLE, POSITION_SELECTION, VELOCITY_SELECTION } bodyCreationState;

	struct Body2DClone
	{
		Body2D* original, clone;
		Body2DClone(Body2D* orig) : original(orig), clone(*orig) {}
		bool operator==(const Body2DClone& b) const { return b.original == this->original; }
		std::string toString() { return clone.toString(); }
	};

	//================================================================================================================================================
	protected:
	struct Physics2DEventsManager; // helper struct to buffer collision events
	Physics2DEventsManager* physicsEventsManager;

	Uint32 pixelDepth;
	long currentIterationCount;
	SDL_Thread* threadPhysics, *threadViewUpdate;
	SDL_mutex* physicsAccessMutex;
	Vector2D bodyCreationPosition, bodyCreationVelocity;
	double bodyCreationDiameter;
	Uint32 lastMouseLeftButtonDown;
	bool isMouseLeftButtonDown;
	Vector2D lastMouseClickPoint;

	//auxiliary variables
	bool auxWasRunningBeforeSelection;
	bool auxWasRunningBeforeBodyCreationMode;

	Vector2D getTransposedNoRef(const Vector2D& position) const;

	void performPhysics(); //updates physics
	void updateView(); //updates view
	void onCollision(std::vector<Body2D*>& collidingList, Body2D& resultingMerger); //overrides Physics2D::CollisionListener

	static int threadFunctionPhysics(void* arg); //thread function
	static int threadFunctionPlanetariumUpdate(void* arg); //thread function
};



#endif /* PLANETARIUM_HPP_ */
