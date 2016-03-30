/*
 * planetarium.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium.hpp"

#include <stdexcept>

#include "util.hpp"
#include "main_window.hpp"
#include "physics/physics2d.hpp"

using CPlanetsGUI::colorToInt;
using CPlanetsGUI::modifyColor;
using std::cout; using std::endl;

int threadFunctionPhysics(void* arg);
int threadFunctionPlanetariumUpdate(void* arg);

Planetarium::Planetarium(WinBase* parentWidget, Rect rect, Id _id)
: WinBase(parentWidget, 0, rect.x, rect.y, rect.w, rect.h, 0, _id),
  physics(new Physics2D()), viewportPosition(), bgColor(SDL_Color()),
  zoom(1.0), minimumBodyRenderingRadius(3.0),
  strokeSizeNormal(1), strokeSizeFocused(2),
  running(false), sleepingTime(25), currentViewportTranlationRate()
{
	modifyColor(bgColor, 0, 0, 0);
	physics->physics2DSolver = new LeapfrogSolver(physics->universe);
	this->threadPhysics = SDL_CreateThread(threadFunctionPhysics, this);
	this->threadViewUpdate = SDL_CreateThread(threadFunctionPlanetariumUpdate, this);
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
	foreach(Body2D*, body, std::vector<Body2D*>, this->physics->universe.bodies)
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

void Planetarium::performPhysics()
{
	for(;true;rest(sleepingTime))
	{
		if(running)
			this->physics->step();
	}
}

void Planetarium::updateView()
{
	for(;true;rest(25)) // XXX Hardcoded time. It should be parameterized.
	{
		this->viewportPosition += this->currentViewportTranlationRate;
		CPlanetsGUI::triggerRepaint();
	}
}

void Planetarium::recolorAllBodies()
{
	foreach(Body2D*, body, std::vector<Body2D*>, this->physics->universe.bodies)
	{
		PlanetariumUserObject* custom = (PlanetariumUserObject*) body->userObject;
		SDL_Color* oldColor = custom->color;
		custom->color = CPlanetsGUI::getRandomColor();
		delete oldColor;
	}
}

void Planetarium::addCustomBody(Body2D* body, SDL_Color* color)
{
	physics->universe.bodies.push_back(body);
	physics->universe.bodies.back()->userObject = new PlanetariumUserObject(color);
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


