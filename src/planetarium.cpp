/*
 * planetarium.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <map>

#include "util.hpp"
#include "SDL_util.hpp"
#include "physics/physics2d.hpp"
#include "physics/physics2d_solvers.hpp"

using std::cout; using std::endl;
using std::vector;
using std::queue;
using std::map;
using SDL_util::colorToInt;
using futil::iterable_queue;
using Math::max;
using Math::min;

//custom data to be carried by each Body2D
struct PlanetariumUserObject
{
	PlanetariumUserObject(SDL_Color* color)
	: color(color)
	{}
	SDL_Color* color;
	~PlanetariumUserObject() { delete color; }
};

struct CollisionEvent
{
	vector<Body2D> collidingBodies;
	Body2D resultingMerger;

	~CollisionEvent()
	{
		foreach(Body2D&, b, vector<Body2D>, collidingBodies)
		{
			delete static_cast<PlanetariumUserObject*>(b.userObject);
		}
	}
};

// helper struct to buffer collision events
struct Planetarium::Physics2DEventsManager
{
	queue< CollisionEvent* > collisionEvents;
	SDL_mutex* mutex;
	Physics2DEventsManager() : collisionEvents(), mutex(SDL_CreateMutex()) {}
	~Physics2DEventsManager() { SDL_DestroyMutex(mutex); }
};

Planetarium::Planetarium(WinBase* parentWidget, Rect rect, Id _id)
: BgrWin(parentWidget, rect, null, null, Planetarium::onMouseDown, null, Planetarium::onMouseUp, 0, _id),
  physics(new Physics2D()), running(false), sleepingTime(DEFAULT_SLEEPING_TIME), fps(DEFAULT_FPS),
  bgColor(SDL_util::Color::BLACK), strokeColorNormal(SDL_util::Color::WHITE), strokeColorFocused(SDL_util::Color::ORANGE),
  strokeSizeNormal(DEFAULT_STROKE_SIZE_NORMAL), strokeSizeFocused(DEFAULT_STROKE_SIZE_FOCUSED),
  isViewportTranslationRateProportionalToZoom(true), pauseOnSelection(true),
  viewportPosition(), viewportZoom(1.0), minimumBodyRenderingRadius(3.0), focusedBodies(), tryAA(false),
  viewportTranlationRateValue(DEFAULT_VIEWPORT_TRANSLATE_RATE), viewportZoomChangeRateValue(DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE),
  currentViewportTranlationRate(), currentViewportZoomChangeRate(1),
  bodyCreationDiameterRatio(DEFAULT_BODY_CREATION_DIAMETER_RATIO), bodyCreationDensity(DEFAULT_BODY_CREATION_DENSITY),
  orbitTracer(this), bodyCreationState(IDLE),
  //protected stuff
  physicsEventsManager(new Physics2DEventsManager()),
  isUpdating(false),
  threadPhysics(SDL_CreateThread(threadFunctionPhysics, this)),
  threadViewUpdate(SDL_CreateThread(threadFunctionPlanetariumUpdate, this)),
  physicsAccessMutex(SDL_CreateMutex()), registeredBodyCollisionListeners(),
  bodyCreationPosition(), bodyCreationVelocity(), bodyCreationDiameter(),
  lastMouseLeftButtonDown(0), isMouseLeftButtonDown(false), lastMouseClickPoint()
{
	this->physics->physics2DSolver = new LeapfrogSolver(physics->universe);
	this->physics->addCollisionListener(this);
}

Planetarium::~Planetarium()
{
	SDL_KillThread(threadPhysics);
	SDL_KillThread(threadViewUpdate);
	SDL_DestroyMutex(physicsAccessMutex);
}

struct CopyBody2D extends Body2D
{
	Body2D* original;
	CopyBody2D(Body2D* orig) : Body2D(*orig), original(orig)  {}
};

void Planetarium::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, colorToInt(this->win, bgColor)); //clears the screen

	int (*circle_function) (SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (tryAA? aacircleRGBA : circleRGBA);
	int (*line_function) (SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (tryAA? aalineRGBA : lineRGBA);

	//make a copy of the current state of bodies.
	std::vector<CopyBody2D> bodies;
	synchronized(physicsAccessMutex)
	{
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
		{
			bodies.push_back(CopyBody2D(body)); //copy the body

			//lets copy the user object for precaution, since its a pointer
			SDL_Color* bodyColor = static_cast<PlanetariumUserObject*>(body->userObject)->color;
			SDL_Color* bodyColorCopy = new SDL_Color();
			*bodyColorCopy = *bodyColor;
			bodies.back().userObject = new PlanetariumUserObject(bodyColorCopy);
		}
	}// end synchronized(physicsAccessMutex)

	//draw all traced orbits (only if tracer is active)
	if(this->orbitTracer.isActive) foreach(CopyBody2D&, body, vector<CopyBody2D>, bodies)
	{
		switch(this->orbitTracer.style)
		{
			case OrbitTracer::POINT: orbitTracer.drawDotted(body.original); break;
			default:case OrbitTracer::LINEAR: orbitTracer.drawLinear(body.original); break;
			case OrbitTracer::SPLINE: orbitTracer.drawQuadricBezier(body.original); break;
		}
	}

	//draw all bodies
	foreach(CopyBody2D&, body, vector<CopyBody2D>, bodies)
	{
		SDL_Color* bodyColor = body.userObject != null? ((PlanetariumUserObject*) body.userObject)->color : null;

		double size = viewportZoom*body.diameter;
		Vector2D v = this->getTransposed(body.position);

		if(size < this->minimumBodyRenderingRadius)
			size = this->minimumBodyRenderingRadius;

		bool isFocused = Collections::containsElement(this->focusedBodies, body.original);

		//if body is focused draw its border with 'strokeSizeFocused' size, otherwise use 'strokeSizeNormal'
		int& strokeSize = isFocused? strokeSizeFocused : strokeSizeNormal;

		//if body is focused draw its border with 'strokeColorFocused' color, otherwise use 'strokeColorNormal'
		SDL_Color& borderColor = isFocused? strokeColorFocused : strokeColorNormal;

		//if stroke size is more than 1px wide, draw a bigger circle to thicken the body border
		if(strokeSize > 1)
			//draw body border (middle part)
			filledCircleRGBA(this->win, v.x, v.y, round(size*0.5) + strokeSize-1, borderColor.r, borderColor.g, borderColor.b, 255);

		//draw body
		if(bodyColor != null) //if there is not body color information, skip
			filledCircleRGBA(this->win, v.x, v.y, round(size*0.5), bodyColor->r, bodyColor->g, bodyColor->b, 255);

		//inner blending with stroke when the stroke is thick
		if(strokeSize > 1)
			circle_function(this->win, v.x, v.y, round(size*0.5), bodyColor->r, bodyColor->g, bodyColor->b, 255);

		//draw body border (if stroke size is more than 1, it is the "border of the border" part)
		circle_function(this->win, v.x, v.y, round(size*0.5) + strokeSize-1, borderColor.r, borderColor.g, borderColor.b, 255);

		//record position
		if(running) //ToDo should this also be avoided when orbitTracer.isActive==false?
			orbitTracer.record(body.original);
	}

	//draw body creation helper stubs
	if(bodyCreationState != IDLE)
	{
		if(bodyCreationState == POSITION_SELECTION)
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			mouseX -= this->area.x; mouseY -= this->area.y;
			circle_function(this->win, mouseX, mouseY, round(this->bodyCreationDiameterRatio*BODY_CREATION_DIAMETER_FACTOR*0.5), 255, 255, 255, 255);
		}
		else if(bodyCreationState == VELOCITY_SELECTION)
		{
			Vector2D newBodyPos = this->getTransposed(bodyCreationPosition);
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			mouseX -= this->area.x; mouseY -= this->area.y;
			circle_function(this->win, newBodyPos.x, newBodyPos.y, round(this->viewportZoom*this->bodyCreationDiameter*0.5), 255, 255, 255, 255);
			line_function(this->win, newBodyPos.x, newBodyPos.y, mouseX, mouseY, 255, 255, 255, 255);
		}
	}
	else if(isMouseLeftButtonDown) // rectangular mouse selection stubs
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		rectangleRGBA(this->win, lastMouseClickPoint.x, lastMouseClickPoint.y, mouseX - area.x, mouseY - area.y, 255, 255, 255, 255);
	}
}

Vector2D Planetarium::getTransposed(const Vector2D& position) const
{
//	cout << "DEBUG: viewport: " << (*(this->viewportPosition)).x << ", " << (*(this->viewportPosition)).y << endl;
	return position.difference(this->viewportPosition).scale(viewportZoom);
}

void Planetarium::setRunning(bool run)
{
	this->running = run;
}

void Planetarium::doRefresh()
{
	draw_blit_upd();
	isUpdating = false;
}

//--------------- \/ \/ SYNCHRONIZED METHODS \/ \/ -----------

void Planetarium::recolorAllBodies()
{
	synchronized(physicsAccessMutex)
	{
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
		{
			PlanetariumUserObject* custom = (PlanetariumUserObject*) body->userObject;
			SDL_Color* oldColor = custom->color;
			custom->color = SDL_util::getRandomColor();
			delete oldColor;
		}
	}
}

void Planetarium::addCustomBody(Body2D* body, SDL_Color* color)
{
	synchronized(physicsAccessMutex)
	{
		physics->universe.bodies.push_back(body);
		physics->universe.bodies.back()->userObject = new PlanetariumUserObject(color);

		//notify listeners about the body created
		foreach(Planetarium::UniverseEventListener*, listener, vector<Planetarium::UniverseEventListener*>, registeredBodyCollisionListeners)
		{
			listener->onBodyCreation(*physics->universe.bodies.back());
		}
	}
}

vector<Body2D> Planetarium::getBodies() const
{
	vector<Body2D> bodies;
	synchronized(physicsAccessMutex)
	{
		const_foreach(Body2D*, i, vector<Body2D*>, physics->universe.bodies)
		{
			bodies.push_back(Body2D(*i)); //must do deep copy on userObject
		}
	}
	return bodies;
}

void Planetarium::setUniverse(Universe2D* u)
{
	const_foreach(Body2D*, i, vector<Body2D*>, u->bodies)
		i->userObject = new PlanetariumUserObject(SDL_util::getRandomColor());

	synchronized(physicsAccessMutex)
	{
		double timeStep = physics->physics2DSolver->timestep; //record current timestep
		physics->universe = *u; //overwrite instance
		delete physics->physics2DSolver; //delete older solver
		physics->physics2DSolver = new LeapfrogSolver(physics->universe); //create new solver
		physics->physics2DSolver->timestep = timeStep; //use previously defined timestep
	}
}

//--------------- /\ /\ SYNCHRONIZED METHODS /\ /\ -----------

void Planetarium::addUniverseEventListener(UniverseEventListener* listener)
{
	//kinda wrong, all instances of Planetarium will share this
	this->registeredBodyCollisionListeners.push_back(listener);
}

void Planetarium::removeUniverseEventListener(UniverseEventListener* listener)
{
	//kinda wrong, all instances of Planetarium will share this
	Collections::removeElement(this->registeredBodyCollisionListeners, listener);
}

Planetarium::OrbitTracer::OrbitTracer(Planetarium* p)
: style(LINEAR), isActive(false), traceLength(20), traces(),
  planetarium(p)
{}

void Planetarium::OrbitTracer::record(Body2D* body)
{
	this->traces[body].push(body->position);
	while(this->traces[body].size() > traceLength)
		this->traces[body].pop();
}

iterable_queue<Vector2D> Planetarium::OrbitTracer::getTrace(Body2D* body)
{
	return this->traces[body];
}

void Planetarium::OrbitTracer::clearTrace(const Body2D* body)
{
	this->traces.erase(const_cast<Body2D*>(body));
}

void Planetarium::OrbitTracer::drawDotted(Body2D* body)
{
	if(body->userObject == null) return; //if there isn't body data, there isn't body color. leave.
	SDL_Color* bodyColor = ((PlanetariumUserObject*) body->userObject)->color;

	iterable_queue<Vector2D> trace = this->getTrace(body);

	foreach(Vector2D&, r, iterable_queue<Vector2D>, trace)
	{
		Vector2D pv = planetarium->getTransposed(r);
		pixelRGBA(planetarium->win, round(pv.x), round(pv.y), bodyColor->r, bodyColor->g, bodyColor->b, 255);
	}
}

void Planetarium::OrbitTracer::drawLinear(Body2D* body)
{
	if(body->userObject == null) return; //if there isn't body data, there isn't body color. leave.
	SDL_Color* bodyColor = ((PlanetariumUserObject*) body->userObject)->color;

	iterable_queue<Vector2D> trace = this->getTrace(body);

	int (*const line_function) (SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (planetarium->tryAA? aalineRGBA : lineRGBA);

	Vector2D previousPosition = trace.front();
	foreach(Vector2D&, recordedPosition, iterable_queue<Vector2D>, trace)
	{
		if(recordedPosition != previousPosition) //avoid drawing segments of same points
		{
			Vector2D recPosTrans = planetarium->getTransposed(recordedPosition), prevPosTrans = planetarium->getTransposed(previousPosition);
			line_function(planetarium->win, round(prevPosTrans.x), round(prevPosTrans.y), round(recPosTrans.x), round(recPosTrans.y), bodyColor->r, bodyColor->g, bodyColor->b, 255);
		}
		previousPosition = recordedPosition;
	}
}

// http://stackoverflow.com/questions/9658932/snappy-bezier-curves
// http://www.ferzkopp.net/Software/SDL_gfx-2.0/Docs/html/_s_d_l__gfx_primitives_8h.html#a7203e3a463da499b5b0cadf211d19ff3
void Planetarium::OrbitTracer::drawQuadricBezier(Body2D* body)
{
	if(body->userObject == null) return; //if there isn't body data, there isn't body color. leave.
	SDL_Color* bodyColor = ((PlanetariumUserObject*) body->userObject)->color;

	iterable_queue<Vector2D> trace = this->getTrace(body);

	Vector2D previousPosition = trace.front();
	Vector2D previousSupport;
//	if(trace.size() > 1) previousSupport = trace.front().times(3).subtract(*(trace.begin()+1)).scale(0.5); //kickstart aux
	if(trace.size() > 1) previousSupport = trace.front().times(2).subtract(*(trace.begin()+1)); //kickstart aux
	foreach(Vector2D&, recordedPosition, iterable_queue<Vector2D>, trace)
	{
		if(recordedPosition != previousPosition) //avoid drawing segments of same points
		{
			//fixme Fix quadratic bezier spline implementation
			Vector2D supportPoint = planetarium->getTransposed(previousSupport);
			Vector2D recPosTrans = planetarium->getTransposed(recordedPosition), prevPosTrans = planetarium->getTransposed(previousPosition);
			Sint16 pxs[] = {static_cast<Sint16>(prevPosTrans.x), static_cast<Sint16>(supportPoint.x), static_cast<Sint16>(recPosTrans.x)};
			Sint16 pys[] = {static_cast<Sint16>(prevPosTrans.y), static_cast<Sint16>(supportPoint.y), static_cast<Sint16>(recPosTrans.y)};
			bezierRGBA(planetarium->win, pxs, pys, 3, 3, bodyColor->r, bodyColor->g, bodyColor->b, 255);
		}
		previousPosition = recordedPosition;
		previousSupport = previousPosition.times(2).subtract(previousSupport);
	}
}

// ===========================  protected stuff =====================================================================================

void Planetarium::performPhysics()
{
	while(true)
	{
		if(running)
		{
			synchronized(physicsAccessMutex)
			{
				this->physics->step();
			}
		}
		SDL_Delay(sleepingTime);
	}
}

void Planetarium::updateView()
{
	static long lastTime;
	while(true)
	{
		if(not physicsEventsManager->collisionEvents.empty()) //notify listeners about the collisions
		{
			SDL_mutex* collisionEventsMutex = physicsEventsManager->mutex;
			synchronized(collisionEventsMutex)
			{
				while(not physicsEventsManager->collisionEvents.empty()) //has collision events
				{
					CollisionEvent* ev = physicsEventsManager->collisionEvents.front(); //gets first
					foreach(UniverseEventListener*, listener, vector<UniverseEventListener*>, registeredBodyCollisionListeners) //for each listener
						listener->onBodyCollision(ev->collidingBodies, ev->resultingMerger); //notify

					physicsEventsManager->collisionEvents.pop();//after using, unregister event
					delete ev; //and delete event object
				}
			}
		}

		//update viewport position
		this->viewportPosition.x += this->currentViewportTranlationRate.x * (this->isViewportTranslationRateProportionalToZoom? 1.0/viewportZoom : 1.0);
		this->viewportPosition.y += this->currentViewportTranlationRate.y * (this->isViewportTranslationRateProportionalToZoom? 1.0/viewportZoom : 1.0);

		//update zoom
		if(this->currentViewportZoomChangeRate != 0)
		{
			double prevZoom = this->viewportZoom;

			//update zoom
			this->viewportZoom *= this->currentViewportZoomChangeRate;

			//center after zooming (if zoom changed)
			this->viewportPosition.x += this->tw_area.w * (1/prevZoom - 1/viewportZoom) * 0.5;
			this->viewportPosition.y += this->tw_area.h * (1/prevZoom - 1/viewportZoom) * 0.5;
		}
		if(not isUpdating)
		{
			isUpdating = true;
			send_uev(USER_EVENT_ID__REDRAW_REQUESTED, this->id.id1);
		}
		lastTime = SDL_GetTicks();
		SDL_Delay(1000/fps - (SDL_GetTicks() - lastTime));
	}
}

// callback called by physics thread
void Planetarium::onCollision(vector<Body2D*>& collidingList, Body2D& resultingMerger)
{
	//reconstructs custom data
	resultingMerger.userObject = new PlanetariumUserObject(new SDL_Color());
	PlanetariumUserObject* obj = (PlanetariumUserObject*) resultingMerger.userObject;
	long double r=0, g=0, b=0, tm=0;
	foreach(Body2D*, body, vector<Body2D*>, collidingList)
	{
		const SDL_Color* bodyColor = static_cast<PlanetariumUserObject*> (body->userObject)->color;
		r += bodyColor->r * body->mass;
		g += bodyColor->g * body->mass;
		b += bodyColor->b * body->mass;
		tm += body->mass;
		orbitTracer.clearTrace(body);
	}
	if(tm==0)tm=1; //safety
	obj->color = new SDL_Color();
	obj->color->r = static_cast<Uint8>(r/tm);
	obj->color->g = static_cast<Uint8>(g/tm);
	obj->color->b = static_cast<Uint8>(b/tm);

	CollisionEvent* ev = new CollisionEvent();

	vector<Body2D>& collidingListCopy = ev->collidingBodies;
	foreach(Body2D*, body, vector<Body2D*>, collidingList)
	{
		PlanetariumUserObject* uobj = static_cast<PlanetariumUserObject*> (body->userObject);
		collidingListCopy.push_back(*body);
		collidingListCopy.back().userObject = new PlanetariumUserObject(new SDL_Color(*(uobj->color))); //implicit copy constructor
		delete uobj; //we need to delete it since it the physics code won't do it (it has no knowledge of this)
	}
	ev->resultingMerger = resultingMerger;

	SDL_mutex* collisionEventsMutex = physicsEventsManager->mutex;
	synchronized(collisionEventsMutex)
	{
		//add collision event to be consumed
		physicsEventsManager->collisionEvents.push(ev); //provided list and merger must be copies
	}
}

void Planetarium::onMouseDown(BgrWin* bgr, int x, int y, int but)
{
	if(but == SDL_BUTTON_LEFT)
	{
		Planetarium* planetarium = static_cast<Planetarium*>(bgr);
		planetarium->lastMouseLeftButtonDown = SDL_GetTicks();
		planetarium->isMouseLeftButtonDown = true;
		planetarium->lastMouseClickPoint = Vector2D(x, y);
		if(planetarium->pauseOnSelection)
			planetarium->setRunning(false);
	}
}

void Planetarium::onMouseUp(BgrWin* bgr, int x, int y, int but)
{
	if(but == SDL_BUTTON_LEFT)
	{
		Planetarium* planetarium = static_cast<Planetarium*>(bgr);

		Vector2D pointedPosition = Vector2D(x, y).scale(1.0/planetarium->viewportZoom).add(planetarium->viewportPosition);
		Vector2D prevPointedPosition = planetarium->lastMouseClickPoint.times(1.0/planetarium->viewportZoom).add(planetarium->viewportPosition);

		if(SDL_GetTicks() - planetarium->lastMouseLeftButtonDown < 250) //click event
		{
			if(planetarium->bodyCreationState == POSITION_SELECTION)
			{
				planetarium->bodyCreationPosition = pointedPosition;
				planetarium->bodyCreationDiameter = (planetarium->bodyCreationDiameterRatio/planetarium->viewportZoom) * BODY_CREATION_DIAMETER_FACTOR;
				planetarium->bodyCreationState = VELOCITY_SELECTION;
				return;
			}
			if(planetarium->bodyCreationState == VELOCITY_SELECTION)
			{
				Vector2D selectedVelocity = pointedPosition.difference(planetarium->bodyCreationPosition);
				double mass = (Math::PI/6.0) * planetarium->bodyCreationDensity * planetarium->bodyCreationDiameter * planetarium->bodyCreationDiameter * planetarium->bodyCreationDiameter;
				Body2D* newBody = new Body2D(mass, planetarium->bodyCreationDiameter, planetarium->bodyCreationPosition, selectedVelocity, Vector2D());
				planetarium->addCustomBody(newBody, SDL_util::getRandomColor());
				planetarium->bodyCreationState = IDLE;
				planetarium->setRunning();
			}
			else //user tried to click a single body, or it was a mistake/random action.
			{
				//check if the clicked point is above a body. if yes, "focused" the body.
				planetarium->focusedBodies.clear();
				SDL_mutex* collisionEventsMutex = planetarium->physicsEventsManager->mutex;
				synchronized(collisionEventsMutex)
				{
					foreach(Body2D*, body, vector<Body2D*>, planetarium->physics->universe.bodies)
					{
						if(body->position.distance(pointedPosition) <= body->diameter*0.5)
						{
							planetarium->focusedBodies.push_back(body);
						}
					}
				}
				if(planetarium->pauseOnSelection)
					planetarium->setRunning();
			}
		}
		else //mouse up after holding down
		{
			//make all bodies under this to be "focused"
			planetarium->focusedBodies.clear();

			double rtix = min(prevPointedPosition.x, pointedPosition.x), rtfx = max(prevPointedPosition.x, pointedPosition.x),
				   rtiy = min(prevPointedPosition.y, pointedPosition.y), rtfy = max(prevPointedPosition.y, pointedPosition.y);

			SDL_mutex* collisionEventsMutex = planetarium->physicsEventsManager->mutex;
			synchronized(collisionEventsMutex)
			{
				foreach(Body2D*, body, vector<Body2D*>, planetarium->physics->universe.bodies)
				{
					if(body->position.x >= rtix && body->position.x <= rtfx
					&& body->position.y >= rtiy && body->position.y <= rtfy)
					{
						planetarium->focusedBodies.push_back(body);
					}
				}
			}
			if(planetarium->pauseOnSelection)
				planetarium->setRunning();
		}
		planetarium->isMouseLeftButtonDown = false;
	}
}

//  -----------------------------  thread functions ------------------------------------------------

int Planetarium::threadFunctionPhysics(void* arg)
{
	try
	{
		((Planetarium*) arg)->performPhysics();
	}
	catch(std::exception& e)
	{
		cout << "bad behavior on physics thread! " << e.what() << endl;
	}
	cout << "physics thread stopped." << endl;
	return 0;
}

int Planetarium::threadFunctionPlanetariumUpdate(void* arg)
{
	try
	{
		((Planetarium*) arg)->updateView();
		cout << "planetarium view update thread stopped." << endl;
	}
	catch(std::exception& e)
	{
		cout << "bad behavior on planetarium view update thread! " << e.what() << endl;
	}
	return 0;
}

