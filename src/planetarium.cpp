/*
 * planetarium.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium.hpp"

#include <SDL/SDL_gfxPrimitives.h>

#include <cfloat>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <map>

#include "SDL_util.hpp"
#include "physics/physics2d.hpp"
#include "physics/solvers/one_step.hpp"

using std::cout;
using std::endl;
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

//helper struct to deal with collision events
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

Planetarium::Planetarium(SDL_Rect rect, Uint32 pixdepth)
: surf(SDL_CreateRGBSurface(SDL_SWSURFACE,rect.w,rect.h,pixdepth,0,0,0,0)), size(rect), pos(rect), isRedrawPending(false),
  physics(new Physics2D()), running(false), stepDelay(DEFAULT_SLEEPING_TIME), fps(DEFAULT_FPS),
  legacyControl(false), displayPeriod(DEFAULT_DISPLAY_PERIOD), iterationsPerDisplay(DEFAULT_ITERATIONS_PER_DISPLAY),
  bgColor(SDL_util::Color::BLACK), strokeColorNormal(SDL_util::Color::WHITE), strokeColorFocused(SDL_util::Color::ORANGE),
  strokeSizeNormal(DEFAULT_STROKE_SIZE_NORMAL), strokeSizeFocused(DEFAULT_STROKE_SIZE_FOCUSED),
  isViewportTranslationRateProportionalToZoom(true), pauseOnSelection(true),
  viewportPosition(), viewportZoom(1.0), minimumBodyRenderingRadius(DEFAULT_MINIMUM_BODY_RENDERING_RADIUS), focusedBodies(), tryAA(false),
  viewportTranlationRateValue(DEFAULT_VIEWPORT_TRANSLATE_RATE), viewportZoomChangeRateValue(DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE),
  currentViewportTranlationRate(), currentViewportZoomChangeRate(1),
  bodyCreationDiameterRatio(DEFAULT_BODY_CREATION_DIAMETER_RATIO),
  bodyCreationDensity(DEFAULT_BODY_CREATION_DENSITY),
  bodyCreationSpeed(DEFAULT_BODY_CREATION_SPEED),
  drawDispatcher(null), listeners(), orbitTracer(this), bodyCreationState(IDLE),
  //protected stuff
  physicsEventsManager(new Physics2DEventsManager()),
  pixelDepth(pixdepth),
  currentIterationCount(0),
  threadPhysics(null),
  threadViewUpdate(null),
  physicsAccessMutex(SDL_CreateMutex()),
  bodyCreationPosition(), bodyCreationVelocity(), bodyCreationDiameter(),
  lastMouseLeftButtonDown(0), isMouseLeftButtonDown(false), lastMouseClickPoint(),
  //aux
  auxWasRunningBeforeSelection(false),
  auxWasRunningBeforeBodyCreationMode(false)

{
	this->physics->solver = new LeapfrogSolver(physics->universe);
	this->physics->listeners.push_back(this);
}

Planetarium::~Planetarium()
{
	SDL_KillThread(threadPhysics);
	SDL_KillThread(threadViewUpdate);
	SDL_DestroyMutex(physicsAccessMutex);
	SDL_FreeSurface(surf);
}

void Planetarium::start()
{
	threadPhysics = SDL_CreateThread(threadFunctionPhysics, this);
	threadViewUpdate = SDL_CreateThread(threadFunctionPlanetariumUpdate, this);
	running = true;
}

void Planetarium::draw()
{
	SDL_FillRect(this->surf, null, colorToInt(this->surf, bgColor)); //clears the screen

	int (*circle_function) (SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (tryAA? aacircleRGBA : circleRGBA);
	int (*line_function) (SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (tryAA? aalineRGBA : lineRGBA);

	//make a copy of the current state of bodies.
	std::vector<Body2DClone> bodies;
	synchronized(physicsAccessMutex)
	{
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
		{
			bodies.push_back(Body2DClone(body)); //copy the body

			//lets copy the user object for precaution, since its a pointer
			SDL_Color* bodyColor = static_cast<PlanetariumUserObject*>(body->userObject)->color;
			SDL_Color* bodyColorCopy = new SDL_Color();
			*bodyColorCopy = *bodyColor;
			bodies.back().clone.userObject = new PlanetariumUserObject(bodyColorCopy);
		}
	}// end synchronized(physicsAccessMutex)

	//draw all traced orbits (only if tracer is active)
	if(this->orbitTracer.isActive) foreach(Body2DClone&, body, vector<Body2DClone>, bodies)
	{
		orbitTracer.drawTrace(body);
	}

	//draw all bodies
	foreach(Body2DClone&, body, vector<Body2DClone>, bodies)
	{
		SDL_Color* bodyColor = body.clone.userObject != null? ((PlanetariumUserObject*) body.clone.userObject)->color : null;

		double size = viewportZoom*body.clone.diameter;
		Vector2D v = this->getTransposed(body.clone.position);

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
			filledCircleRGBA(this->surf, v.x, v.y, round(size*0.5) + strokeSize-1, borderColor.r, borderColor.g, borderColor.b, 255);

		//draw body
		if(bodyColor != null) //if there is not body color information, skip
			filledCircleRGBA(this->surf, v.x, v.y, round(size*0.5), bodyColor->r, bodyColor->g, bodyColor->b, 255);

		//inner blending with stroke when the stroke is thick
		if(strokeSize > 1)
			circle_function(this->surf, v.x, v.y, round(size*0.5), bodyColor->r, bodyColor->g, bodyColor->b, 255);

		//draw body border (if stroke size is more than 1, it is the "border of the border" part)
		circle_function(this->surf, v.x, v.y, round(size*0.5) + strokeSize-1, borderColor.r, borderColor.g, borderColor.b, 255);

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
			mouseX -= this->pos.x; mouseY -= this->pos.y;
			circle_function(this->surf, mouseX, mouseY, round(this->bodyCreationDiameterRatio*BODY_CREATION_DIAMETER_FACTOR*0.5), 255, 255, 255, 255);
		}
		else if(bodyCreationState == VELOCITY_SELECTION)
		{
			Vector2D newBodyPos = this->getTransposed(bodyCreationPosition);
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			mouseX -= this->pos.x; mouseY -= this->pos.y;
			circle_function(this->surf, newBodyPos.x, newBodyPos.y, round(this->viewportZoom*this->bodyCreationDiameter*0.5), 255, 255, 255, 255);
			line_function(this->surf, newBodyPos.x, newBodyPos.y, mouseX, mouseY, 255, 255, 255, 255);
		}
	}
	else if(isMouseLeftButtonDown) // rectangular mouse selection stubs
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		rectangleRGBA(this->surf, lastMouseClickPoint.x, lastMouseClickPoint.y, mouseX - pos.x, mouseY - pos.y, 255, 255, 255, 255);
	}
}

void Planetarium::widen(int dx, int dy)
{
	SDL_FreeSurface(surf);
	surf=SDL_CreateRGBSurface(SDL_SWSURFACE,size.w+dx,size.h+dy,pixelDepth,0,0,0,0);
	size.w += dx; size.h += dy;
}

Vector2D Planetarium::getTransposed(const Vector2D& position) const
{
//	cout << "DEBUG: viewport: " << (*(this->viewportPosition)).x << ", " << (*(this->viewportPosition)).y << endl;
	return (position - physics->referenceFrame.position() - viewportPosition)*viewportZoom;
}

Vector2D Planetarium::getAntiTransposed(const Vector2D& position) const
{
	return position*(1.0/viewportZoom) + viewportPosition + physics->referenceFrame.position();
}

//protected
Vector2D Planetarium::getTransposedNoRef(const Vector2D& position) const
{
	return (position - viewportPosition)*viewportZoom;
}

void Planetarium::setRunning(bool run)
{
	this->running = run;
}

void Planetarium::setBodyCreationMode(bool enable)
{
	if(enable)
	{
		bodyCreationState = POSITION_SELECTION;
		auxWasRunningBeforeBodyCreationMode = running;
		setRunning(false);
	}
	else this->bodyCreationState = IDLE;
}

void Planetarium::setFocusedBodies(Body2D*const* bodyarr, unsigned n)
{
	focusedBodies.clear();
	for(unsigned i = 0; i < n; i++)
	{
		focusedBodies.push_back(bodyarr[i]);
	}
}

void Planetarium::setFocusedBodies(const vector<Body2D*>& bodies)
{
	focusedBodies.clear();
	const_foreach(Body2D*, body, vector<Body2D*>, bodies)
	{
		focusedBodies.push_back(body);
	}
}

//------------------------------------ \/ \/ SYNCHRONIZED METHODS \/ \/ -----------------------------------

void Planetarium::removeFocusedBodies(bool alsoDelete)
{
	synchronized(physicsAccessMutex)
	{
		foreach(Body2D*, body, vector<Body2D*>, focusedBodies)
		{
			Collections::removeElement(physics->universe.bodies, body);
		}
	}

	foreach(Body2D*, body, vector<Body2D*>, focusedBodies)
	{
		//notify listeners about the body deleted
		for(unsigned i = 0; i < listeners.size(); i++)
			listeners[i]->onBodyDeletion(body);

		orbitTracer.clearTrace(body);

		if(alsoDelete) delete body;
	}

	focusedBodies.clear();
}

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

/** Adds (safely) a custom body. If no color is specified, a random color will be used. */
void Planetarium::addCustomBody(Body2D* body, SDL_Color* color)
{
	if(color == null)
		color = SDL_util::getRandomColor();

	synchronized(physicsAccessMutex)
	{
		physics->universe.bodies.push_back(body);
		physics->universe.bodies.back()->userObject = new PlanetariumUserObject(color);

		//notify listeners about the body created
		for(unsigned i = 0; i < listeners.size(); i++)
			listeners[i]->onBodyCreation(*physics->universe.bodies.back());
	}
}

/** Adds (safely) a custom body with the given parameters. If no color is specified, a random color will be used. */
void Planetarium::addCustomBody(double mass, double diameter, const Vector2D& position, const Vector2D& velocity, SDL_Color* color)
{
	addCustomBody(new Body2D(mass, diameter, position, velocity, Vector2D()), color);
}

/** Adds a random body with resulting characteristics being, on average, the given parameters. If an area is specified, the resulting body will be positioned randomly within it.*/
void Planetarium::addRandomBody(double avMass, double avDiameter, double avSpeed, const double area[4])
{
	const double minPosX = area != null ? area[0] : DBL_MIN,
				 minPosY = area != null ? area[1] : DBL_MIN,
				 maxPosX = area != null ? area[0] + area[2] : DBL_MAX,
				 maxPosY = area != null ? area[1] + area[3] : DBL_MAX;

	addCustomBody(
		Math::randomNormalBetween(avMass*0.9, avMass*1.1),
		Math::randomNormalBetween(avDiameter*0.9, avDiameter*1.1),
		Vector2D(Math::randomBetween(minPosX, maxPosX), Math::randomBetween(minPosY, maxPosY)),
		Vector2D(Math::randomNormalBetween(avSpeed*0.9, avSpeed*1.1))
	);
}

/** Adds a random body with random traits. If an area is specified, the resulting body will be positioned randomly within it.*/
void Planetarium::addRandomBody(const double area[4])
{
	double az = 1/viewportZoom;
	double diameter = (bodyCreationDiameterRatio * az) * BODY_CREATION_DIAMETER_FACTOR;
	double mass = (Math::PI/6.0) * bodyCreationDensity * pow(diameter, 3);
	double speed = bodyCreationSpeed * az;

	addRandomBody(mass, diameter, speed, area);
}

/** Adds a random body with random traits. If an area is specified, the resulting body will be positioned randomly within it.*/
void Planetarium::addRandomOrbitingBody(const double area[4])
{
	double az = 1/viewportZoom;
	double diameter = (bodyCreationDiameterRatio * az) * BODY_CREATION_DIAMETER_FACTOR;
	diameter = Math::randomNormalBetween(diameter*0.9, diameter*1.1);
	double mass = (Math::PI/6.0) * bodyCreationDensity * pow(diameter, 3);
	mass = Math::randomNormalBetween(mass*0.9, mass*1.1);

	const double minPosX = area != null ? area[0] : DBL_MIN,
				 minPosY = area != null ? area[1] : DBL_MIN,
				 maxPosX = area != null ? area[0] + area[2] : DBL_MAX,
				 maxPosY = area != null ? area[1] + area[3] : DBL_MAX;

	Vector2D position(Math::randomBetween(minPosX, maxPosX), Math::randomBetween(minPosY, maxPosY));

	Vector2D centerOfMass, centerOfMassVelocity;
	double totalMass;

	getCurrentOrbitalReference(centerOfMass, centerOfMassVelocity, totalMass);

	Vector2D distanceToCenter = position.difference(centerOfMass);

	double speed = sqrt((2*physics->universe.gravity*totalMass)/distanceToCenter.length());
	speed = Math::randomNormalBetween(speed*0.7, speed*0.9);
	Vector2D velocity = distanceToCenter.unit().perpendicular().scale(speed);
	velocity.add(centerOfMassVelocity);

	addCustomBody(mass, diameter, position, velocity);

	//TODO set velocity to orbit: ve = sqrt(2GM/r) when adding orbiting body
	//http://physics.stackexchange.com/questions/227502/orbital-mechanics-will-a-satellite-crash?rq=1
}

void Planetarium::removeBody(Body2D* body, bool alsoDelete)
{
	synchronized(physicsAccessMutex)
	{
		Collections::removeElement(physics->universe.bodies, body);
	}

	Collections::removeElement(focusedBodies, body);

	//notify listeners about the body deleted
	for(unsigned i = 0; i < listeners.size(); i++)
		listeners[i]->onBodyDeletion(body);

	orbitTracer.clearTrace(body);

	if(alsoDelete) delete body;
}

vector<Planetarium::Body2DClone> Planetarium::getBodies() const
{
	vector<Body2DClone> bodies;
	synchronized(physicsAccessMutex)
	{
		const_foreach(Body2D*, i, vector<Body2D*>, physics->universe.bodies)
		{
			bodies.push_back(Body2DClone(i)); //must do deep copy on userObject
		}
	}
	return bodies;
}

void Planetarium::setUniverse(const Universe2D& u)
{
	const_foreach(Body2D*, i, vector<Body2D*>, u.bodies)
		if(i->userObject == null)
			i->userObject = new PlanetariumUserObject(SDL_util::getRandomColor());

	synchronized(physicsAccessMutex)
	{
		//delete the user objects of the current universe, as setUniverse() will delete the current universe, but not its user objects
		foreach(Body2D*, oldBody, vector<Body2D*>, physics->universe.bodies)
			delete static_cast<PlanetariumUserObject*>(oldBody->userObject);

		physics->setUniverse(u);
	}
}

long double Planetarium::getTotalKineticEnergy() const
{
	long double value;
	synchronized(physicsAccessMutex)
	{
		value = physics->totalKineticEnergy;
	}
	return value;
}

long double Planetarium::getTotalPotentialEnergy() const
{
	long double value;
	synchronized(physicsAccessMutex)
	{
		value = physics->totalPotentialEnergy;
	}
	return value;
}

unsigned Planetarium::getBodyCount() const
{
	unsigned value;
	synchronized(physicsAccessMutex)
	{
		value = physics->bodyCount;
	}
	return value;
}

//---------------------------------- /\ /\ SYNCHRONIZED METHODS /\ /\ ------------------------------------------

void Planetarium::onMouseButtonPressed(int x, int y, int but)
{
	if(but == SDL_BUTTON_LEFT)
	{
		lastMouseLeftButtonDown = SDL_GetTicks();
		isMouseLeftButtonDown = true;
		lastMouseClickPoint = Vector2D(x, y);
		auxWasRunningBeforeSelection = running;
		if(pauseOnSelection)
			setRunning(false);
	}
}

void Planetarium::onMouseButtonReleased(int x, int y, int but)
{
	bool notToogling = not(SDL_GetModState() & KMOD_CTRL); // check if click-wise selection
	if(but == SDL_BUTTON_LEFT)
	{
		Vector2D pointedPosition = getAntiTransposed(Vector2D(x, y));
		Vector2D prevPointedPosition = getAntiTransposed(lastMouseClickPoint);

		if(SDL_GetTicks() - lastMouseLeftButtonDown < 250) //click event
		{
			if(bodyCreationState == POSITION_SELECTION)
			{
				bodyCreationPosition = pointedPosition;
				bodyCreationDiameter = (bodyCreationDiameterRatio/viewportZoom) * BODY_CREATION_DIAMETER_FACTOR;
				bodyCreationState = VELOCITY_SELECTION;
				return;
			}
			if(bodyCreationState == VELOCITY_SELECTION)
			{
				Vector2D selectedVelocity = pointedPosition.difference(bodyCreationPosition);
				double mass = (Math::PI/6.0) * bodyCreationDensity * bodyCreationDiameter * bodyCreationDiameter * bodyCreationDiameter;
				Body2D* newBody = new Body2D(mass, bodyCreationDiameter, bodyCreationPosition, selectedVelocity, Vector2D());
				addCustomBody(newBody, SDL_util::getRandomColor());
				setBodyCreationMode(IDLE);
				if(auxWasRunningBeforeBodyCreationMode)
					setRunning();
			}
			else //user tried to click a single body, or it was a mistake/random action.
			{
				// Check if the clicked point is above a body. if yes, "focused" the body.

				if(notToogling)
					focusedBodies.clear();

				SDL_mutex* collisionEventsMutex = physicsEventsManager->mutex;
				synchronized(collisionEventsMutex)
				{
					foreach(Body2D*, body, vector<Body2D*>, physics->universe.bodies)
					{
						if(body->position.distance(pointedPosition) <= body->diameter*0.5)
						{
							if(notToogling) //set focused
								focusedBodies.push_back(body);
							else //toogle focus
								if(Collections::removeElement(focusedBodies, body) == false)
									focusedBodies.push_back(body);
						}
					}
				}
				if(pauseOnSelection and auxWasRunningBeforeSelection)
					setRunning();

				//notify listeners about re-focusing of bodies
				for(unsigned i = 0; i < listeners.size(); i++)
					listeners[i]->onBodyReFocus();
			}
		}
		else //mouse up after holding down
		{
			// Make all bodies under this to be "focused"

			if(notToogling)
				focusedBodies.clear();

			double rtix = min(prevPointedPosition.x, pointedPosition.x), rtfx = max(prevPointedPosition.x, pointedPosition.x),
				   rtiy = min(prevPointedPosition.y, pointedPosition.y), rtfy = max(prevPointedPosition.y, pointedPosition.y);

			SDL_mutex* collisionEventsMutex = physicsEventsManager->mutex;
			synchronized(collisionEventsMutex)
			{
				foreach(Body2D*, body, vector<Body2D*>, physics->universe.bodies)
				{
					if(body->position.x >= rtix && body->position.x <= rtfx
					&& body->position.y >= rtiy && body->position.y <= rtfy)
					{
						if(notToogling) //set focused
							focusedBodies.push_back(body);
						else //toogle focus
							if(Collections::removeElement(focusedBodies, body) == false)
								focusedBodies.push_back(body);
					}
				}
			}
			if(pauseOnSelection and auxWasRunningBeforeSelection)
				setRunning();

			//notify listeners about re-focusing of bodies
			for(unsigned i = 0; i < listeners.size(); i++)
				listeners[i]->onBodyReFocus();
		}
		isMouseLeftButtonDown = false;
	}
}

Planetarium::OrbitTracer::OrbitTracer(Planetarium* p)
: style(LINEAR), isActive(false), traceLength(20), traces(),
  planetarium(p)
{}

void Planetarium::OrbitTracer::record(Body2D* body)
{
	this->traces[body].push(body->position-planetarium->physics->referenceFrame.position());
	while(this->traces[body].size() > traceLength)
		this->traces[body].pop();
}

iterable_queue<Vector2D>& Planetarium::OrbitTracer::getTrace(Body2D* body)
{
	return this->traces[body];
}

void Planetarium::OrbitTracer::clearTrace(const Body2D* body)
{
	this->traces.erase(const_cast<Body2D*>(body));
}

void Planetarium::OrbitTracer::drawTrace(Body2D* body)
{
	if(body->userObject == null) return; //if there isn't body data, there isn't body color. leave.
	SDL_Color* bodyColor = ((PlanetariumUserObject*) body->userObject)->color;

	iterable_queue<Vector2D>& trace = this->getTrace(body);

	drawTrace(trace, bodyColor);
}

void Planetarium::OrbitTracer::drawTrace(Body2DClone& body)
{
	if(body.clone.userObject == null) return; //if there isn't body data, there isn't body color. leave.
	SDL_Color* bodyColor = ((PlanetariumUserObject*) body.clone.userObject)->color;

	iterable_queue<Vector2D>& trace = this->getTrace(body.original); //get original body trace

	drawTrace(trace, bodyColor);
}

void Planetarium::OrbitTracer::drawTrace(iterable_queue<Vector2D>& trace, SDL_Color* color)
{
	switch(style)
	{
		case DOTTED:			drawDotted(trace, color); break;
		default:case LINEAR:	drawLinear(trace, color); break;
		case SPLINE:			drawQuadricBezier(trace, color); break;
	}
}

void Planetarium::OrbitTracer::drawDotted(iterable_queue<Vector2D>& trace, SDL_Color* bodyColor)
{
	foreach(Vector2D&, r, iterable_queue<Vector2D>, trace)
	{
		Vector2D pv = planetarium->getTransposedNoRef(r);
		pixelRGBA(planetarium->surf, round(pv.x), round(pv.y), bodyColor->r, bodyColor->g, bodyColor->b, 255);
	}
}

void Planetarium::OrbitTracer::drawLinear(iterable_queue<Vector2D>& trace, SDL_Color* bodyColor)
{
	int (*const line_function) (SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (planetarium->tryAA? aalineRGBA : lineRGBA);

	Vector2D previousPosition = trace.front();
	foreach(Vector2D&, recordedPosition, iterable_queue<Vector2D>, trace)
	{
		if(recordedPosition != previousPosition) //avoid drawing segments of same points
		{
			Vector2D recPosTrans = planetarium->getTransposedNoRef(recordedPosition), prevPosTrans = planetarium->getTransposedNoRef(previousPosition);
			line_function(planetarium->surf, round(prevPosTrans.x), round(prevPosTrans.y), round(recPosTrans.x), round(recPosTrans.y), bodyColor->r, bodyColor->g, bodyColor->b, 255);
		}
		previousPosition = recordedPosition;
	}
}

// http://stackoverflow.com/questions/9658932/snappy-bezier-curves
// http://www.ferzkopp.net/Software/SDL_gfx-2.0/Docs/html/_s_d_l__gfx_primitives_8h.html#a7203e3a463da499b5b0cadf211d19ff3
void Planetarium::OrbitTracer::drawQuadricBezier(iterable_queue<Vector2D>& trace, SDL_Color* bodyColor)
{
	Vector2D previousPosition = trace.front();
	Vector2D previousSupport;
//	if(trace.size() > 1) previousSupport = trace.front().times(3).subtract(*(trace.begin()+1)).scale(0.5); //kickstart aux
	if(trace.size() > 1) previousSupport = trace.front().times(2).subtract(*(trace.begin()+1)); //kickstart aux
	foreach(Vector2D&, recordedPosition, iterable_queue<Vector2D>, trace)
	{
		if(recordedPosition != previousPosition) //avoid drawing segments of same points
		{
			//fixme Fix quadratic bezier spline implementation
			Vector2D supportPoint = planetarium->getTransposedNoRef(previousSupport);
			Vector2D recPosTrans = planetarium->getTransposedNoRef(recordedPosition), prevPosTrans = planetarium->getTransposedNoRef(previousPosition);
			Sint16 pxs[] = {static_cast<Sint16>(prevPosTrans.x), static_cast<Sint16>(supportPoint.x), static_cast<Sint16>(recPosTrans.x)};
			Sint16 pys[] = {static_cast<Sint16>(prevPosTrans.y), static_cast<Sint16>(supportPoint.y), static_cast<Sint16>(recPosTrans.y)};
			bezierRGBA(planetarium->surf, pxs, pys, 3, 3, bodyColor->r, bodyColor->g, bodyColor->b, 255);
		}
		previousPosition = recordedPosition;
		previousSupport = previousPosition.times(2).subtract(previousSupport);
	}
}

// ===========================  protected stuff =====================================================================================

void Planetarium::performPhysics()
{
	AbstractPhysics2DSolver* previousSolver = null;
	while(true)
	{
		if(running && (not legacyControl || currentIterationCount++ < iterationsPerDisplay))
		{
			synchronized(physicsAccessMutex)
			{
				this->physics->step();
			}
		}
		SDL_Delay(legacyControl? 0 : stepDelay);

		//debug
		if(previousSolver != this->physics->solver)
		{
			cout << "Now using " << physics->solver->factory->solverDisplayName << " solver..." << endl;;
			previousSolver = this->physics->solver;
		}
	}
}

void Planetarium::updateView()
{
	static long lastUpdateTime, lastRedrawRequestTime = 0;
	while(true)
	{
		lastUpdateTime = SDL_GetTicks();
		if(not physicsEventsManager->collisionEvents.empty()) //notify listeners about the collisions
		{
			SDL_mutex* collisionEventsMutex = physicsEventsManager->mutex;
			synchronized(collisionEventsMutex)
			{
				while(not physicsEventsManager->collisionEvents.empty()) //has collision events
				{
					CollisionEvent* ev = physicsEventsManager->collisionEvents.front(); //gets first
					for(unsigned i = 0; i < listeners.size(); i++)
						listeners[i]->onBodyCollision(ev->collidingBodies, ev->resultingMerger); //notify

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
			this->viewportPosition.x += this->size.w * (1/prevZoom - 1/viewportZoom) * 0.5;
			this->viewportPosition.y += this->size.h * (1/prevZoom - 1/viewportZoom) * 0.5;
		}

		const bool allowedByLegacy = not running or
				(currentIterationCount >= ((long) iterationsPerDisplay) and (SDL_GetTicks() - lastRedrawRequestTime) > ((long) displayPeriod));

		if(not isRedrawPending && (not legacyControl || allowedByLegacy) )
		{
			isRedrawPending = true;

			if(drawDispatcher != null)
				drawDispatcher->onSurfaceUpdate();
			else
				draw();

			//to aid legacy controls
			currentIterationCount = 0;
			lastRedrawRequestTime = SDL_GetTicks();
		}

		SDL_Delay(1000/fps - (SDL_GetTicks() - lastUpdateTime));
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

void Planetarium::getCurrentOrbitalReference(Vector2D& position, Vector2D& velocity, double& mass)
{
	if(physics->referenceFrame.isPointLike())
	{
		vector<Planetarium::Body2DClone> bodies = getBodies();
		if(bodies.size() == 0)
		{
			position = velocity = Vector2D::NULL_VECTOR;
			mass = 0;
		}
		else if(bodies.size() == 1)
		{
			position = bodies[0].clone.position;
			velocity = bodies[0].clone.velocity;
			mass = bodies[0].clone.mass;
		}
		else
		{
			position.scale(0);
			velocity.scale(0);
			for(unsigned i = 0; i < bodies.size(); i++)
			{
				position.add(bodies[0].clone.position.times(bodies[0].clone.mass));
				velocity.add(bodies[0].clone.velocity.times(bodies[0].clone.mass));
				mass += bodies[0].clone.mass;
			}

			if(mass != 0)
			{
				position.scale(1.0/mass);
				velocity.scale(1.0/mass);
			}
		}
	}
	else
	{
		position = physics->referenceFrame.position();
		velocity = physics->referenceFrame.velocity();
		mass = physics->referenceFrame.mass();
	}
}

//  -----------------------------  thread functions ------------------------------------------------

int Planetarium::threadFunctionPhysics(void* arg)
{
	try { ((Planetarium*) arg)->performPhysics(); }
	catch(std::exception& e)
	{
		cout << "bad behavior on physics thread! " << e.what() << endl;
	}
	cout << "physics thread stopped." << endl;
	return 0;
}

int Planetarium::threadFunctionPlanetariumUpdate(void* arg)
{
	try { ((Planetarium*) arg)->updateView(); }
	catch(std::exception& e)
	{
		cout << "bad behavior on planetarium view update thread! " << e.what() << endl;
	}
	cout << "planetarium view update thread stopped." << endl;
	return 0;
}
