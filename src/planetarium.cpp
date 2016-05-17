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
#include <utility>

#include "util.hpp"
#include "SDL_util.hpp"
#include "physics/physics2d.hpp"
#include "physics/physics2d_solvers.hpp"

using std::cout; using std::endl;
using std::vector;
using std::queue;
using std::pair;
using std::map;
using SDL_util::colorToInt;
using SDL_util::getRandomColor;
using futil::iterable_queue;

//custom data to be carried by each Body2D
struct PlanetariumUserObject
{
	PlanetariumUserObject(SDL_Color* color)
	: color(color)
	{}
	SDL_Color* color;
};

// helper struct to buffer collision events
struct Planetarium::Physics2DEventsManager
{
	queue< pair<vector<Body2D>, Body2D>* > collisionEvents;
	SDL_mutex* mutex;
	Physics2DEventsManager() : collisionEvents(), mutex(SDL_CreateMutex()) {}
	~Physics2DEventsManager() { SDL_DestroyMutex(mutex); }
};

Planetarium::Planetarium(WinBase* parentWidget, Rect rect, Id _id)
: BgrWin(parentWidget, rect, null, null, Planetarium::onMouseDown, null, Planetarium::onMouseUp, 0, _id),
  physics(new Physics2D()), running(false), sleepingTime(DEFAULT_SLEEPING_TIME), fps(DEFAULT_FPS),
  bgColor(SDL_util::Color::BLACK), strokeSizeNormal(DEFAULT_STROKE_SIZE_NORMAL), strokeSizeFocused(DEFAULT_STROKE_SIZE_FOCUSED),
  isViewportTranslationRateProportionalToZoom(true),
  viewportPosition(), viewportZoom(1.0), minimumBodyRenderingRadius(3.0), tryAA(false),
  viewportTranlationRateValue(DEFAULT_VIEWPORT_TRANSLATE_RATE), viewportZoomChangeRateValue(DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE),
  currentViewportTranlationRate(), currentViewportZoomChangeRate(1),
  bodyCreationDiameterRatio(DEFAULT_BODY_CREATION_DIAMETER_RATIO), bodyCreationDensity(DEFAULT_BODY_CREATION_DENSITY),
  orbitTracer(), bodyCreationState(IDLE),
  //protected stuff
  physicsEventsManager(new Physics2DEventsManager()),
  isUpdating(false),
  threadPhysics(SDL_CreateThread(threadFunctionPhysics, this)),
  threadViewUpdate(SDL_CreateThread(threadFunctionPlanetariumUpdate, this)),
  physicsAccessMutex(SDL_CreateMutex()), registeredBodyCollisionListeners(),
  bodyCreationPosition(), bodyCreationVelocity(), bodyCreationDiameter(),
  lastMouseLeftButtonDown(0)
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

void Planetarium::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, colorToInt(this->win, bgColor)); //clears the screen

	//draw all stuff
	synchronized(physicsAccessMutex)
	{
		//draw all traced orbits (only if tracer is active)
		if(this->orbitTracer.isActive) foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
		{
			if(body->userObject == null) goto break1; //if there isn't body data, there isn't body color. leave.
			SDL_Color* bodyColor = ((PlanetariumUserObject*) body->userObject)->color;

			iterable_queue<Vector2D> trace = orbitTracer.getTrace(body);
			switch(this->orbitTracer.style)
			{
				case OrbitTracer::POINT:
				{
					foreach(Vector2D&, r, iterable_queue<Vector2D>, trace)
					{
						Vector2D pv = this->getTransposed(r);
						pixelRGBA(this->win, round(pv.x), round(pv.y), bodyColor->r, bodyColor->g, bodyColor->b, 255);
					}
					break;
				}

				case OrbitTracer::LINEAR:
				{
					Vector2D previousPosition = trace.front();
					foreach(Vector2D&, recordedPosition, iterable_queue<Vector2D>, trace)
					{
						if(recordedPosition != previousPosition) //avoid drawing segments of same points
						{
							Vector2D recPosTrans = this->getTransposed(recordedPosition), prevPosTrans = this->getTransposed(previousPosition);
							if(not tryAA)
								lineRGBA(this->win, round(prevPosTrans.x), round(prevPosTrans.y), round(recPosTrans.x), round(recPosTrans.y), bodyColor->r, bodyColor->g, bodyColor->b, 255);
							else
								aalineRGBA(this->win, round(prevPosTrans.x), round(prevPosTrans.y), round(recPosTrans.x), round(recPosTrans.y), bodyColor->r, bodyColor->g, bodyColor->b, 255);
						}
						previousPosition = recordedPosition;
					}
					break;
				}

				case OrbitTracer::SPLINE:
				{
					Vector2D previousPosition = trace.front();
					foreach(Vector2D&, recordedPosition, iterable_queue<Vector2D>, trace)
					{
						if(recordedPosition != previousPosition) //avoid drawing segments of same points
						{
							//FixMe Fix quadratic bezier spline implementation
							Vector2D recPosTrans = this->getTransposed(recordedPosition), prevPosTrans = this->getTransposed(previousPosition);
							Vector2D supportPoint;// = ???
							Sint16 pxs[] = {static_cast<Sint16>(prevPosTrans.x), static_cast<Sint16>(supportPoint.x), static_cast<Sint16>(recPosTrans.x)};
							Sint16 pys[] = {static_cast<Sint16>(prevPosTrans.y), static_cast<Sint16>(supportPoint.y), static_cast<Sint16>(recPosTrans.y)};
							bezierRGBA(this->win, pxs, pys, 2, 3, bodyColor->r, bodyColor->g, bodyColor->b, 255);
						}
						previousPosition = recordedPosition;
					}
					break;
				}

				default:break;
			}
		}
		break1:

		//draw all bodies
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
		{
			SDL_Color* bodyColor = body->userObject != null? ((PlanetariumUserObject*) body->userObject)->color : null;

			double size = viewportZoom*body->diameter;
			Vector2D v = this->getTransposed(body->position);

			if(size < this->minimumBodyRenderingRadius)
				size = this->minimumBodyRenderingRadius;

			//draw body
			if(bodyColor != null)
				filledCircleRGBA(this->win, v.x, v.y, round(size*0.5), bodyColor->r, bodyColor->g, bodyColor->b, 255);

			//draw body border. todo choose other body border color when focused
			if(not tryAA)
				circleRGBA(this->win, v.x, v.y, round(size*0.5), 255, 255, 255, 255);
			else
				aacircleRGBA(this->win, v.x, v.y, round(size*0.5), 255, 255, 255, 255);

			//record position
			if(running) //ToDo should this also be avoided when orbitTracer.isActive==false?
				orbitTracer.record(body, body->position);
		}
	} // end synchronized(physicsAccessMutex)

	//draw body creation helper stubs
	if(bodyCreationState != IDLE)
	{
		if(bodyCreationState == POSITION_SELECTION)
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			mouseX -= this->area.x; mouseY -= this->area.y;
			if(not tryAA)
				circleRGBA(this->win, mouseX, mouseY, round(this->bodyCreationDiameterRatio*BODY_CREATION_DIAMETER_FACTOR*0.5), 255, 255, 255, 255);
			else
				aacircleRGBA(this->win, mouseX, mouseY, round(this->bodyCreationDiameterRatio*BODY_CREATION_DIAMETER_FACTOR*0.5), 255, 255, 255, 255);
		}
		else if(bodyCreationState == VELOCITY_SELECTION)
		{
			Vector2D newBodyPos = this->getTransposed(bodyCreationPosition);
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			mouseX -= this->area.x; mouseY -= this->area.y;
			if(not tryAA)
			{
				circleRGBA(this->win, newBodyPos.x, newBodyPos.y, round(this->viewportZoom*this->bodyCreationDiameter*0.5), 255, 255, 255, 255);
				lineRGBA(this->win, newBodyPos.x, newBodyPos.y, mouseX, mouseY, 255, 255, 255, 255);
			}
			else
			{
				aacircleRGBA(this->win, newBodyPos.x, newBodyPos.y, round(this->viewportZoom*this->bodyCreationDiameter*0.5), 255, 255, 255, 255);
				aalineRGBA(this->win, newBodyPos.x, newBodyPos.y, mouseX, mouseY, 255, 255, 255, 255);
			}
		}
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
			custom->color = getRandomColor();
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
	std::vector<Body2D> bodies;
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

Planetarium::OrbitTracer::OrbitTracer()
: style(LINEAR), isActive(false), traceLength(20), traces()
{}

void Planetarium::OrbitTracer::record(Body2D* body, Vector2D& position)
{
	this->traces[body].push(position);
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
				pair<vector<Body2D>, Body2D>* ev; //event
				while(not physicsEventsManager->collisionEvents.empty()) //has collision events
				{
					ev = physicsEventsManager->collisionEvents.front(); //gets first
					foreach(UniverseEventListener*, listener, vector<UniverseEventListener*>, registeredBodyCollisionListeners) //for each listener
						listener->onBodyCollision(ev->first, ev->second); //notify

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
	resultingMerger.userObject = new PlanetariumUserObject(new SDL_Color);
	PlanetariumUserObject* obj = (PlanetariumUserObject*) resultingMerger.userObject;
	long r=0, g=0, b=0;
	foreach(Body2D*, body, vector<Body2D*>, collidingList)
	{
		r += ((PlanetariumUserObject*) body->userObject)->color->r;
		g += ((PlanetariumUserObject*) body->userObject)->color->g;
		b += ((PlanetariumUserObject*) body->userObject)->color->b;
		orbitTracer.clearTrace(body);
	}
	obj->color = new SDL_Color;
	obj->color->r = r/collidingList.size();
	obj->color->g = g/collidingList.size();
	obj->color->b = b/collidingList.size();

	typedef pair<vector<Body2D>, Body2D> Event;
	Event* ev = new Event();

	vector<Body2D>& collidingListCopy = ev->first;
	foreach(Body2D*, i, vector<Body2D*>, collidingList)
	{
		collidingListCopy.push_back(*i);
		collidingListCopy.back().userObject = new PlanetariumUserObject(new SDL_Color);
	}
	ev->second = resultingMerger;

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
	}

}

void Planetarium::onMouseUp(BgrWin* bgr, int x, int y, int but)
{
	if(but == SDL_BUTTON_LEFT)
	{
		Planetarium* planetarium = static_cast<Planetarium*>(bgr);
		Vector2D pointedPosition = Vector2D(x, y).scale(1.0/planetarium->viewportZoom).add(planetarium->viewportPosition);
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
				//TODO set velocity to orbit: ve = sqrt(2GM/r) when adding orbiting body
				Body2D* newBody = new Body2D(mass, planetarium->bodyCreationDiameter, planetarium->bodyCreationPosition, selectedVelocity, Vector2D());
				planetarium->addCustomBody(newBody, SDL_util::getRandomColor());
				planetarium->bodyCreationState = IDLE;
				planetarium->setRunning();
			}
		}
		else //mouse up after holding down
		{}
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

