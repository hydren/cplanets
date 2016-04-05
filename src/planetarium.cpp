/*
 * planetarium.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium.hpp"

#include <stdexcept>
#include <vector>
#include <queue>

#include "util.hpp"
#include "main_window.hpp"
#include "physics/physics2d.hpp"
#include "physics/physics2d_solvers.hpp"

using CPlanetsGUI::colorToInt;
using CPlanetsGUI::modifyColor;
using std::cout; using std::endl;
using std::vector;

int threadFunctionPhysics(void* arg);
int threadFunctionPlanetariumUpdate(void* arg);
void bodyCollisionCallback(vector<Body2D*>& collidingList, Body2D& resultingMerger);

struct UniverseCollisionEvent
{
	UniverseCollisionEvent(vector<Body2D> collidingList, Body2D resultingMerger)
	: collidingList(collidingList), resultingMerger(resultingMerger) {}
	vector<Body2D> collidingList;
	Body2D resultingMerger;
};
//kinda wrong, all instances of Planetarium will share this
std::queue<UniverseCollisionEvent> collisionEvents;
SDL_mutex* collisionEventsMutex = null;

Planetarium::Planetarium(WinBase* parentWidget, Rect rect, Id _id)
: WinBase(parentWidget, 0, rect.x, rect.y, rect.w, rect.h, 0, _id),
  physics(new Physics2D()), running(false), sleepingTime(DEFAULT_SLEEPING_TIME),
  bgColor(SDL_Color()), strokeSizeNormal(DEFAULT_STROKE_SIZE_NORMAL), strokeSizeFocused(DEFAULT_STROKE_SIZE_FOCUSED),
  viewportPosition(), viewportZoom(1.0), minimumBodyRenderingRadius(3.0),
  viewportTranlationRateValue(DEFAULT_VIEWPORT_TRANSLATE_RATE), viewportZoomChangeRateValue(DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE),
  currentViewportTranlationRate(), currentViewportZoomChangeRate(1)
{
	modifyColor(this->bgColor, 0, 0, 0);
	this->physics->physics2DSolver = new LeapfrogSolver(physics->universe);
	this->physics->onPhysics2DBodyCollision = bodyCollisionCallback;
	this->threadPhysics = SDL_CreateThread(threadFunctionPhysics, this);
	this->threadViewUpdate = SDL_CreateThread(threadFunctionPlanetariumUpdate, this);
	this->physicsAccessMutex = SDL_CreateMutex();
	if(collisionEventsMutex == null) collisionEventsMutex = SDL_CreateMutex(); //XXX not thread-safe
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

	//draw bodies
	SDL_mutexP(this->physicsAccessMutex);
	foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
	{
		double size = viewportZoom*body->diameter;
		if(size < this->minimumBodyRenderingRadius) size = this->minimumBodyRenderingRadius;

		Vector2D v = this->getTransposed(body->position);

		if(body->userObject != null)
		{
			SDL_Color* bodyColor = ((PlanetariumUserObject*) body->userObject)->color;
			filledCircleColor(this->win, v.x, v.y, round(size*0.5), colorToInt(null, *bodyColor, true));
		}

		int borderColor = 0xffffffff; //XXX should use colorToInt() or be a const

//		if(focusedBodies.contains(body))
//			borderColor = int value for orange

		circleColor(this->win, v.x, v.y, round(size*0.5), borderColor);
	}
	SDL_mutexV(this->physicsAccessMutex);
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

//--------------- \/ \/ SYNCHRONIZED METHODS \/ \/ -----------

void Planetarium::recolorAllBodies()
{
	SDL_mutexP(this->physicsAccessMutex);
	foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
	{
		PlanetariumUserObject* custom = (PlanetariumUserObject*) body->userObject;
		SDL_Color* oldColor = custom->color;
		custom->color = CPlanetsGUI::getRandomColor();
		delete oldColor;
	}
	SDL_mutexV(this->physicsAccessMutex);
}

void Planetarium::addCustomBody(Body2D* body, SDL_Color* color)
{
	SDL_mutexP(this->physicsAccessMutex);
	physics->universe.bodies.push_back(body);
	physics->universe.bodies.back()->userObject = new PlanetariumUserObject(color);

	//notify listeners about the body created
	foreach(Planetarium::UniverseEventListener*, listener, vector<Planetarium::UniverseEventListener*>, registeredBodyCollisionListeners)
	{
		listener->onBodyCreation(*physics->universe.bodies.back());
	}
	SDL_mutexV(this->physicsAccessMutex);
}

vector<Body2D> Planetarium::getBodies() const
{
	SDL_mutexP(this->physicsAccessMutex);
	std::vector<Body2D> bodies;
	const_foreach(Body2D*, i, vector<Body2D*>, physics->universe.bodies)
	{
		bodies.push_back(Body2D(*i)); //must do deep copy on userObject
	}
	SDL_mutexV(this->physicsAccessMutex);
	return bodies;
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

void Planetarium::performPhysics()
{
	for(;true;rest(sleepingTime))
	{
		if(running)
		{
			SDL_mutexP(this->physicsAccessMutex);

			this->physics->step();

			SDL_mutexV(this->physicsAccessMutex);
		}
	}
}

void Planetarium::updateView()
{
	for(;true;rest(25)) // XXX Hardcoded time. It should be parameterized.
	{
		if(!collisionEvents.empty()) //notify listeners about the collisions
		{
			SDL_mutexP(collisionEventsMutex);

			for(UniverseCollisionEvent& ev = collisionEvents.front(); !collisionEvents.empty(); collisionEvents.pop())
				foreach(UniverseEventListener*, listener, vector<UniverseEventListener*>, registeredBodyCollisionListeners)
					listener->onBodyCollision(ev.collidingList, ev.resultingMerger);

			SDL_mutexV(collisionEventsMutex);
		}

		//update viewport position
		this->viewportPosition += this->currentViewportTranlationRate;

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
		CPlanetsGUI::triggerRepaint();
	}
}

Planetarium::PlanetariumUserObject::PlanetariumUserObject(SDL_Color* color)
: color(color)
{}

//  -----------------------------  thread functions ------------------------------------------------
int threadFunctionPhysics(void* arg)
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

int threadFunctionPlanetariumUpdate(void* arg)
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

// callback called by physics thread
void bodyCollisionCallback(vector<Body2D*>& collidingList, Body2D& resultingMerger)
{
	//reconstructs custom data
	resultingMerger.userObject = new Planetarium::PlanetariumUserObject(new SDL_Color);
	Planetarium::PlanetariumUserObject* obj = (Planetarium::PlanetariumUserObject*) resultingMerger.userObject;
	long r=0, g=0, b=0;
	foreach(Body2D*, body, vector<Body2D*>, collidingList)
	{
		r += ((Planetarium::PlanetariumUserObject*) body->userObject)->color->r;
		g += ((Planetarium::PlanetariumUserObject*) body->userObject)->color->g;
		b += ((Planetarium::PlanetariumUserObject*) body->userObject)->color->b;
	}
	obj->color = new SDL_Color;
	obj->color->r = r/collidingList.size();
	obj->color->g = g/collidingList.size();
	obj->color->b = b/collidingList.size();

	vector<Body2D> collidingListCopy;
	foreach(Body2D*, i, vector<Body2D*>, collidingList)
	{
		collidingListCopy.push_back(*i);
		collidingListCopy.back().userObject = new Planetarium::PlanetariumUserObject(new SDL_Color);
	}

	SDL_mutexP(collisionEventsMutex);

	//add collision event to be consumed
	collisionEvents.push(UniverseCollisionEvent(collidingListCopy, resultingMerger)); //provided list and merger must be copies

	SDL_mutexV(collisionEventsMutex);
}


