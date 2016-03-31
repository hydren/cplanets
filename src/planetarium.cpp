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

Planetarium::Planetarium(WinBase* parentWidget, Rect rect, Id _id)
: WinBase(parentWidget, 0, rect.x, rect.y, rect.w, rect.h, 0, _id),
  physics(new Physics2D()), viewportPosition(), bgColor(SDL_Color()),
  zoom(1.0), minimumBodyRenderingRadius(3.0),
  strokeSizeNormal(1), strokeSizeFocused(2),
  running(false), sleepingTime(25), currentViewportTranlationRate()
{
	modifyColor(this->bgColor, 0, 0, 0);
	this->physics->physics2DSolver = new LeapfrogSolver(physics->universe);
	this->physics->onPhysics2DBodyCollision = bodyCollisionCallback;
	this->threadPhysics = SDL_CreateThread(threadFunctionPhysics, this);
	this->threadViewUpdate = SDL_CreateThread(threadFunctionPlanetariumUpdate, this);
	this->physicsAccessMutex = SDL_CreateMutex();
}

Planetarium::~Planetarium()
{
	SDL_KillThread(threadPhysics);
	SDL_KillThread(threadViewUpdate);
}

void Planetarium::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, null, colorToInt(this->win, bgColor)); //clears the screen

	//draw bodies
	foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
	{
		double size = zoom*body->diameter;
		if(size < this->minimumBodyRenderingRadius) size = this->minimumBodyRenderingRadius;
		SDL_Color* bodyColor = ((PlanetariumUserObject*) body->userObject)->color;

		Vector2D v = this->getTransposed(body->position);

//		SDL_Rect tmpRect; tmpRect.x = x; tmpRect.y = y; tmpRect.h = size, tmpRect.w = size;
//		SDL_FillRect(this->surface, &tmpRect, colorToInt(*bodyColor));

		filledCircleColor(this->win, v.x, v.y, round(size*0.5), colorToInt(null, *bodyColor, true));

		int borderColor = 0xffffffff;

//		if(focusedBodies.contains(body))
//		{
//			graphics2d.setStroke(focusedStroke);
//			borderColor = int value for orange
//		}

		circleColor(this->win, v.x, v.y, round(size*0.5), borderColor);
	}
}

Vector2D Planetarium::getTransposed(const Vector2D& position) const
{
//	cout << "DEBUG: viewport: " << (*(this->viewportPosition)).x << ", " << (*(this->viewportPosition)).y << endl;
	return position.difference(this->viewportPosition).scale(zoom);
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
		bodies.push_back(Body2D(*i));
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
			for(UniverseCollisionEvent& ev = collisionEvents.front(); !collisionEvents.empty(); collisionEvents.pop())
				foreach(UniverseEventListener*, listener, vector<UniverseEventListener*>, registeredBodyCollisionListeners)
					listener->onBodyCollision(ev.collidingList, ev.resultingMerger);

		this->viewportPosition += this->currentViewportTranlationRate;
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

void bodyCollisionCallback(vector<Body2D*>& collidingList, Body2D& resultingMerger)
{
	//reconstructs custom data
	if(resultingMerger.userObject == null)
	{
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
	}

	vector<Body2D> collidingListCopy;
	foreach(Body2D*, i, vector<Body2D*>, collidingList)
	{
		collidingListCopy.push_back(*i);
	}
	Body2D resultingMergerCopy(resultingMerger);

	//add collision event to be consumed
	collisionEvents.push(UniverseCollisionEvent(collidingListCopy, resultingMergerCopy)); //provided list and merger must be deep copies
	UniverseCollisionEvent& c = collisionEvents.back(); //FixMe this instance is dirty. why?
}


