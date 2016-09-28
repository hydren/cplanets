/*
 * planetarium.cpp
 *
 *  Created on: 1 de fev de 2016
 *      Author: carlosfaruolo
 */

#include "planetarium.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <queue>

#include <cfloat>

#include <SDL/SDL_gfxPrimitives.h>

#include "SDL_util.hpp"

#include "futil/exception.hpp"
#include "futil/math/round.h"
#include "futil/math/more_random.h"
#include "futil/math/gauss_random.h"
#include "futil/math/round.h"
#include "futil/collection/actions.hpp"

#include "physics/physics2d.hpp"
#include "physics/solvers/one_step.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::deque;
using std::map;
using SDL_util::colorToInt;

#define max std::max
#define min std::min

// Deletes inserted planetarium user objects from the given set of bodies.
static void purgeUserObjects(vector<Body2D*>& bodies);
static void purgeUserObjects(vector<Body2D>& bodies);
static void purgeUserObjects(vector<Planetarium::Body2DClone>& bodies, bool onOriginal=false);

//custom data to be carried by each Body2D
struct PlanetariumUserObject
{
	SDL_Color color;
	PlanetariumUserObject() {}
	PlanetariumUserObject(const SDL_Color& color) : color(color) {}
};

//helper struct to deal with collision events
struct CollisionEvent
{
	vector<Body2D> collidingBodies;
	Body2D resultingMerger;

	~CollisionEvent()
	{
		purgeUserObjects(collidingBodies);
	}
};

// helper struct to buffer collision events
struct Planetarium::Physics2DEventsManager
{
	queue<CollisionEvent*> collisionEvents;
	SDL_mutex* mutex;
	Physics2DEventsManager() : collisionEvents(), mutex(SDL_CreateMutex()) {}
	~Physics2DEventsManager() { SDL_DestroyMutex(mutex); }
};

struct Planetarium::StateManager
{
	enum ChangeType { ADDITION, REMOVAL, MERGE };

	struct Change
	{
		vector<Body2DClone> backup;
		vector<Body2DClone> diff;
		ChangeType type;

		Change (const Universe2D& u, const vector<Body2D*>& diff, ChangeType type)
		: backup(), diff(), type(type)
		{
			if(type == MERGE) throw_exception("Invalid StateChange type for constructor! MERGE cannot be used with this constructor!");

			const_foreach(Body2D*, body, vector<Body2D*>, u.bodies)
				backup.push_back(Body2DClone(body));

			const_foreach(Body2D*, body, vector<Body2D*>, diff)
				this->diff.push_back(Body2DClone(body));
		}

		Change (const Universe2D& u, Body2D* diff, ChangeType type)
		: backup(), diff(), type(type)
		{
			if(type == MERGE) throw_exception("Invalid StateChange type for constructor! MERGE cannot be used with this constructor!");

			const_foreach(Body2D*, body, vector<Body2D*>, u.bodies)
				backup.push_back(Body2DClone(body));

			this->diff.push_back(Body2DClone(diff));
		}

		Change (const vector<Body2D*>& merged, Body2D* merger)
		: backup(), diff(), type(MERGE)
		{
			diff.push_back(Body2DClone(merger));
			const_foreach(Body2D*, body, vector<Body2D*>, merged)
				diff.push_back(Body2DClone(body));
		}
	};

	Planetarium* planetarium;
	deque<Change> changes;
	unsigned nonMergeChangesCount;

	StateManager(Planetarium* planetarium) : planetarium(planetarium), changes(), nonMergeChangesCount(0) {}

	void commitAddition(const Universe2D& u, const vector<Body2D*>& diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, ADDITION));
		nonMergeChangesCount++;
		//cout << "commit add" << endl;
	}

	void commitAddition(const Universe2D& u, Body2D* diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, ADDITION));
		nonMergeChangesCount++;
		//cout << "commit add" << endl;
	}

	void commitRemoval(const Universe2D& u, const vector<Body2D*>& diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, REMOVAL));
		nonMergeChangesCount++;
		//cout << "commit rem" << endl;
	}

	void commitRemoval(const Universe2D& u, Body2D* diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, REMOVAL));
		nonMergeChangesCount++;
		//cout << "commit rem" << endl;
	}

	void commitMerge(const vector<Body2D*>& merged, Body2D* merger)
	{
		shrinkToFit();
		changes.push_back(Change(merged, merger));
		//cout << "commit merge" << endl;
	}

	/// Swaps 'oldAdress' pointers in the changes stack (history) for 'newAddress' pointers
	void fixReferences(const Body2D* oldAddress, Body2D* newAddress)
	{
		foreach(Change&, change, deque<Change>, changes)
		{
			foreach(Body2DClone&, backupBody, vector<Body2DClone>, change.backup)
				if(backupBody.original == oldAddress)
					backupBody.original = newAddress;

			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
				if(diffBody.original == oldAddress)
					diffBody.original = newAddress;
		}
	}

	private:void purgeAsUndone(Change& change)
	{
		//cout << "purging " << (change.type==MERGE? "merge":"non-merge") << " as undone" << endl;
		if(change.type == ADDITION)
		{
			//deletes previously added bodies' user objects and the bodies' themselves.
			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
			{
				planetarium->orbitTracer.clearTrace(diffBody.original);
				delete static_cast<PlanetariumUserObject*>(diffBody.original->userObject);
				delete diffBody.original;
			}
		}
		else if(change.type == MERGE)
		{
			//deletes merger's user object and the merger's itself.
			planetarium->orbitTracer.clearTrace(change.diff[0].original);
			delete static_cast<PlanetariumUserObject*>(change.diff[0].original->userObject);
			delete change.diff[0].original;
		}
	}

	private:void purgeAsForgotten(Change& change)
	{
		//cout << "purging " << (change.type==MERGE? "merge":"non-merge") << " as forgotten" << endl;
		if(change.type == REMOVAL)
		{
			//deletes previously removed bodies' user objects and the bodies' themselves.
			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
			{
				planetarium->orbitTracer.clearTrace(diffBody.original);
				delete static_cast<PlanetariumUserObject*>(diffBody.original->userObject);
				delete diffBody.original;
			}
		}
		else if(change.type == MERGE)
		{
			change.diff.erase(change.diff.begin()); //remove merger from the diff list

			//deletes merged bodies' user objects and themselves.
			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
			{
				planetarium->orbitTracer.clearTrace(diffBody.original);
				delete static_cast<PlanetariumUserObject*>(diffBody.original->userObject);
				delete diffBody.original;
			}
		}
	}

	private:void rollbackPositions(Change& state)
	{
		foreach(Body2DClone&, backupBody, vector<Body2DClone>, state.backup)
			foreach(Body2D*, body, vector<Body2D*>, planetarium->physics->universe.bodies)
				if(body == backupBody.original)
					*body = backupBody.clone; // copy attributes (position, velocities, etc...)
	}

	private:void rollbackAddition(Change& state, bool rewind=false)
	{
		rollbackPositions(state);

		foreach(Body2DClone&, addedBody, vector<Body2DClone>, state.diff)
			if (not rewind)
				remove_element(planetarium->physics->universe.bodies, addedBody.original);
			else
				*(addedBody.original) = addedBody.clone; // copy attributes (position, velocities, etc...)
	}

	private:void rollbackRemoval(Change& state, bool rewind=false)
	{
		rollbackPositions(state);
		if(not rewind)
			foreach(Body2DClone&, removedBody, vector<Body2DClone>, state.diff)
				planetarium->physics->universe.bodies.push_back(removedBody.original);
	}

	private:void rollbackMerge(Change& state)
	{
		Body2D* merger = state.diff[0].original;
		remove_element(planetarium->physics->universe.bodies, merger);

		foreach(Body2DClone&, removedBody, vector<Body2DClone>, state.diff)
			if(removedBody.original != merger) //merger is in the first index, ignore it
				planetarium->physics->universe.bodies.push_back(removedBody.original);
	}

	public:
	void rollback(bool rewind=false)
	{
		if(nonMergeChangesCount == 0) return;
		//cout << (rewind? "rewind":"rollback") << endl;
		SDL_mutex* physicsAccessMutex = planetarium->physicsAccessMutex;

		if(changes.back().type != MERGE) // no merge involved, simpler case
		{
			Change& change = changes.back();
			if(change.type == ADDITION)
			{
				synchronized(physicsAccessMutex)
				{
					rollbackAddition(change, rewind);
				}

				if(not rewind) foreach(Body2DClone&, addedBody, vector<Body2DClone>, change.diff)
				{
					//notify listeners about the body deleted
					for(unsigned i = 0; i < planetarium->listeners.size(); i++)
						planetarium->listeners[i]->onBodyDeletion(addedBody.original);
				}
			}

			else if (change.type == REMOVAL)
			{
				synchronized(physicsAccessMutex)
				{
					rollbackRemoval(change, rewind);
				}

				if(not rewind) foreach(Body2DClone&, removedBody, vector<Body2DClone>, change.diff)
				{
					for(unsigned i = 0; i < planetarium->listeners.size(); i++)
						planetarium->listeners[i]->onBodyCreation(*removedBody.original);
				}
			}

			if(not rewind)
			{
				purgeAsUndone(change);
				changes.pop_back(); //stuack pop
				if(change.type != MERGE) nonMergeChangesCount--;
			}
		}

		else //merges happened after last body addition/removal, needs to rollback more than one state
		{
			vector<Body2DClone> removedAddedBodies, addedRemovedBodies; //write down to notify or delete later
			bool needsRollback = true; //when rollbacking merge changes, keep rollbacking futher

			synchronized(physicsAccessMutex)
			{
				while(not changes.empty() and needsRollback)
				{
					Change& change = changes.back();

					if(change.type == ADDITION)
					{
						rollbackAddition(change, rewind);
						if(not rewind) removedAddedBodies.insert(removedAddedBodies.end(), change.diff.begin(), change.diff.end()); //write down to notify or delete later
						needsRollback = false;
					}

					else if (change.type == REMOVAL)
					{
						rollbackRemoval(change, rewind);
						if(not rewind) addedRemovedBodies.insert(addedRemovedBodies.end(), change.diff.begin(), change.diff.end()); //write down to notify later
						needsRollback = false;
					}

					else if (change.type == MERGE)
					{
						rollbackMerge(change);
						removedAddedBodies.push_back(change.diff[0]); //merger ("diff[0]") was removed
						addedRemovedBodies.insert(addedRemovedBodies.end(), change.diff.begin()+1, change.diff.end()); //mergeds were re-added
						needsRollback = true;
					}

					if(change.type == MERGE or not rewind)
					{
						changes.pop_back(); //stack pop
						if(change.type != MERGE) nonMergeChangesCount--;
					}
				}
			}

			//notify listeners about the body removed
			foreach(Body2DClone&, removedBody, vector<Body2DClone>, removedAddedBodies)
			{
				for(unsigned i = 0; i < planetarium->listeners.size(); i++)
					planetarium->listeners[i]->onBodyDeletion(removedBody.original);

				//delete unused manually
				planetarium->orbitTracer.clearTrace(removedBody.original);
				delete static_cast<PlanetariumUserObject*>(removedBody.original->userObject);
				delete removedBody.original;
			}

			//notify listeners about the body re-added
			foreach(Body2DClone&, addedBody, vector<Body2DClone>, addedRemovedBodies)
				for(unsigned i = 0; i < planetarium->listeners.size(); i++)
					planetarium->listeners[i]->onBodyCreation(*addedBody.original);
		}
	}

	void reset()
	{
		while(not changes.empty())
		{
			purgeAsUndone(changes.back());
			changes.pop_back(); //stack pop
		}
		nonMergeChangesCount = 0;
	}

	void shrinkToFit()
	{
		while(changes.size() > planetarium->undoStackMaxSize-1)
		{
			purgeAsForgotten(changes.front());
			if(changes.front().type != MERGE) nonMergeChangesCount--;
			changes.pop_front();
		}
	}

	/// Prints current change stack
	void debug()
	{
		cout << "# debug-stack #" << endl;
		foreach(Change&, change, deque<Change>, changes)
		{
			cout << "@change " << &change << endl;

			cout << "-->backup: ";
			foreach(Body2DClone&, backupBody, vector<Body2DClone>, change.backup)
				cout << backupBody.original << ", ";
			cout << endl;

			cout << "-->diff: ";
			for(unsigned i = 0; i < change.diff.size(); i++)
				cout << change.diff[i].original << (i==0 and change.type==MERGE? "(merger)":"") << ", ";
			cout << endl;
		}
		cout << "# " << nonMergeChangesCount << " non-merge changes #" << endl;
	}
};

const double Planetarium::BODY_CREATION_DIAMETER_FACTOR = 32.0; //refinement factor

const unsigned Planetarium::DEFAULT_VIEWPORT_TRANSLATE_RATE = 8;
const double Planetarium::DEFAULT_VIEWPORT_ZOOM_CHANGE_RATE = 0.1;
const double Planetarium::DEFAULT_BODY_CREATION_DIAMETER_RATIO = 1.0;
const double Planetarium::DEFAULT_BODY_CREATION_DENSITY = 1.0;
const double Planetarium::DEFAULT_BODY_CREATION_SPEED = 20.0;
const unsigned Planetarium::DEFAULT_STROKE_SIZE_NORMAL = 1, Planetarium::DEFAULT_STROKE_SIZE_FOCUSED = 2;
const double Planetarium::DEFAULT_MINIMUM_BODY_RENDERING_RADIUS = 2.0;
const unsigned Planetarium::DEFAULT_SLEEPING_TIME = 25;
const short Planetarium::DEFAULT_FPS = 60;
const long Planetarium::DEFAULT_DISPLAY_PERIOD = 30, Planetarium::DEFAULT_ITERATIONS_PER_DISPLAY = 2;
const unsigned Planetarium::DEFAULT_UNDO_STACK_SIZE = 256;

const bool Planetarium::DEFAULT_ADD_RANDOM_ORBITING_ORIENTATION = true; // the value doesn't really matter, we just need to identify it was used.

Planetarium::Planetarium(SDL_Rect rect, Uint32 pixdepth)
: surf(SDL_CreateRGBSurface(SDL_SWSURFACE,rect.w,rect.h,pixdepth,0,0,0,0)), size(rect), pos(rect), isRedrawPending(false),
  physics(new Physics2D()), running(false), stepDelay(DEFAULT_SLEEPING_TIME), fps(DEFAULT_FPS),
  legacyControl(false), displayPeriod(DEFAULT_DISPLAY_PERIOD), iterationsPerDisplay(DEFAULT_ITERATIONS_PER_DISPLAY),
  rocheLimitComputingEnabled(false), undoStackMaxSize(DEFAULT_UNDO_STACK_SIZE),
  bgColor(SDL_util::Color::BLACK), strokeColorNormal(SDL_util::Color::WHITE),
  strokeColorFocused(SDL_util::Color::ORANGE), strokeColorRocheLimit(SDL_util::Color::RED),
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
  stateManager(new StateManager(this)),
  pixelDepth(pixdepth),
  currentIterationCount(0),
  threadPhysics(null),
  threadViewUpdate(null),
  physicsAccessMutex(SDL_CreateMutex()),
  bodyCreationPosition(), bodyCreationVelocity(), bodyCreationDiameter(),
  lastMouseLeftButtonDown(0), isMouseLeftButtonDown(false), lastMouseClickPoint(),
  undoEnabled(true),
  //aux
  auxWasRunningBeforeSelection(false),
  auxWasRunningBeforeBodyCreationMode(false)

{
	this->physics->solver = new LeapfrogSolver(*physics);
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
	int (*triangle_function) (SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (tryAA? aatrigonRGBA : trigonRGBA);

	//make a copy of the current state of bodies.
	std::vector<Body2DClone> bodies;
	synchronized(physicsAccessMutex)
	{
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
		{
			bodies.push_back(Body2DClone(body)); //copy the body

			//lets copy the user object for precaution, since its a pointer
			SDL_Color& bodyColor = static_cast<PlanetariumUserObject*>(body->userObject)->color;
			bodies.back().clone.userObject = new PlanetariumUserObject(bodyColor);
		}
	}// end synchronized(physicsAccessMutex)

	//draw all traced orbits (only if tracer is active)
	if(this->orbitTracer.isActive) foreach(Body2DClone&, body, vector<Body2DClone>, bodies)
	{
		orbitTracer.drawTrace(body);
	}

	Vector2D primaryPosition, dummy;
	double primaryMass;
	if(rocheLimitComputingEnabled)
		getCurrentOrbitalReference(primaryPosition, dummy, primaryMass);

	//draw all bodies
	foreach(Body2DClone&, body, vector<Body2DClone>, bodies)
	{
		SDL_Color* bodyColor = (body.clone.userObject == null? null : &static_cast<PlanetariumUserObject*>(body.clone.userObject)->color);

		double size = viewportZoom*body.clone.diameter;
		Vector2D v = this->getTransposed(body.clone.position);

		if(size < this->minimumBodyRenderingRadius)
			size = this->minimumBodyRenderingRadius;

		bool isFocused = contains_element(this->focusedBodies, body.original);

		//if body is focused draw its border with 'strokeSizeFocused' size, otherwise use 'strokeSizeNormal'
		int& strokeSize = isFocused? strokeSizeFocused : strokeSizeNormal;

		//if body is focused draw its border with 'strokeColorFocused' color, otherwise use 'strokeColorNormal'
		SDL_Color& borderColor = isFocused? strokeColorFocused : rocheLimitComputingEnabled and isPastRocheLimit(body.clone, primaryPosition, primaryMass)? strokeColorRocheLimit : strokeColorNormal;

		//if stroke size is more than 1px wide, draw a bigger circle to thicken the body border
		if(strokeSize > 1)
			//draw body border (middle part)
			filledCircleRGBA(this->surf, v.x, v.y, round(size*0.5) + strokeSize-1, borderColor.r, borderColor.g, borderColor.b, 255);

		//draw body
		if(bodyColor != null) //if there is not body color information, skip
			filledCircleRGBA(this->surf, v.x, v.y, round(size*0.5), bodyColor->r, bodyColor->g, bodyColor->b, 255);

		//inner blending with stroke when the stroke is thick
		if(bodyColor != null and strokeSize > 1 and size > 3)
			circle_function(this->surf, v.x, v.y, round(size*0.5), bodyColor->r, bodyColor->g, bodyColor->b, 255);

		//draw body border (if stroke size is more than 1, it is the "border of the border" part)
		if(size > 3)
			circle_function(this->surf, v.x, v.y, round(size*0.5) + strokeSize-1, borderColor.r, borderColor.g, borderColor.b, 255);
		else
			circleRGBA(this->surf, v.x, v.y, round(size*0.5) + strokeSize-1, borderColor.r, borderColor.g, borderColor.b, 255);

		//record position
		if(running) //ToDo should this also be avoided when orbitTracer.isActive==false?
			orbitTracer.record(body);
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

			const Vector2D diff = !(Vector2D(mouseX, mouseY) - (newBodyPos));
			Vector2D arrowPointCenter(mouseX, mouseY), arrowPointLeft(mouseX, mouseY), arrowpointRight(mouseX, mouseY);
			arrowPointCenter -= diff*6.0;
			(arrowPointLeft  -= diff*8.0) -= (diff.perpendicular() * 3.0);
			(arrowpointRight -= diff*8.0) += (diff.perpendicular() * 3.0);
			filledTrigonRGBA(this->surf,  mouseX, mouseY, arrowPointLeft.x, arrowPointLeft.y, arrowPointCenter.x, arrowPointCenter.y, 255, 255, 255, 255);
			filledTrigonRGBA(this->surf,  mouseX, mouseY, arrowPointCenter.x, arrowPointCenter.y, arrowpointRight.x, arrowpointRight.y, 255, 255, 255, 255);
			triangle_function(this->surf, mouseX, mouseY, arrowPointLeft.x, arrowPointLeft.y, arrowPointCenter.x, arrowPointCenter.y, 255, 255, 255, 255);
			triangle_function(this->surf, mouseX, mouseY, arrowPointCenter.x, arrowPointCenter.y, arrowpointRight.x, arrowpointRight.y, 255, 255, 255, 255);
		}
	}
	else if(isMouseLeftButtonDown) // rectangular mouse selection stubs
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		rectangleRGBA(this->surf, lastMouseClickPoint.x, lastMouseClickPoint.y, mouseX - pos.x, mouseY - pos.y, 255, 255, 255, 255);
	}

	// delete temporary user objects
	purgeUserObjects(bodies);
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
		if(undoEnabled) stateManager->commitRemoval(this->physics->universe, focusedBodies);
		foreach(Body2D*, body, vector<Body2D*>, focusedBodies)
		{
			remove_element(physics->universe.bodies, body);
		}
	}

	foreach(Body2D*, body, vector<Body2D*>, focusedBodies)
	{
		//notify listeners about the body deleted
		for(unsigned i = 0; i < listeners.size(); i++)
			listeners[i]->onBodyDeletion(body);

		physics->referenceFrame.dissociate(body);

		if(not undoEnabled)
		{
			orbitTracer.clearTrace(body);

			if(alsoDelete)
			{
				delete static_cast<PlanetariumUserObject*>(body->userObject);
				delete body;
			}
		}
	}
	focusedBodies.clear();
}

void Planetarium::removeBody(Body2D* body, bool alsoDelete)
{
	synchronized(physicsAccessMutex)
	{
		if(undoEnabled) stateManager->commitRemoval(this->physics->universe, body);
		remove_element(physics->universe.bodies, body);
	}

	//notify listeners about the body deleted
	for(unsigned i = 0; i < listeners.size(); i++)
		listeners[i]->onBodyDeletion(body);

	physics->referenceFrame.dissociate(body);
	remove_element(focusedBodies, body);

	if(not undoEnabled)
	{
		orbitTracer.clearTrace(body);
		if(alsoDelete)
		{
			delete static_cast<PlanetariumUserObject*>(body->userObject);
			delete body;
		}
	}
}


/** Removes (and deletes) all bodies from the universe. */
void Planetarium::removeAllBodies()
{
	vector<Body2D*> removedBodiesPointers;
	synchronized(physicsAccessMutex)
	{
		if(undoEnabled) stateManager->commitRemoval(this->physics->universe, this->physics->universe.bodies);

		// write down all bodies
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
			removedBodiesPointers.push_back(body);

		//detatch bodies/remove references
		this->physics->universe.bodies.clear();
	}

	focusedBodies.clear();
	physics->referenceFrame.reset();

	foreach(Body2D*, body, vector<Body2D*>, removedBodiesPointers)
	{
		//notify listeners about the body deleted
		for(unsigned i = 0; i < listeners.size(); i++)
			listeners[i]->onBodyDeletion(body);

		if(not undoEnabled)
		{
			orbitTracer.clearTrace(body);

			//trash it
			delete static_cast<PlanetariumUserObject*>(body->userObject);
			delete body;
		}
	}
}

/** Adds (safely) a custom body. If no user object is specified, a new one will be created with a random color. */
void Planetarium::addCustomBody(Body2D* body)
{
	if(body->userObject == null)
		body->userObject = new PlanetariumUserObject(SDL_util::getRandomColor());

	synchronized(physicsAccessMutex)
	{
		if(undoEnabled) stateManager->commitAddition(this->physics->universe, body);
		physics->universe.bodies.push_back(body);
	}

	//notify listeners about the body created
	for(unsigned i = 0; i < listeners.size(); i++)
		listeners[i]->onBodyCreation(*body);
}

/** Adds (safely) a custom body. If no color is specified, a random color will be used. */
void Planetarium::addCustomBody(Body2D* body, const SDL_Color& color)
{
	if(body->userObject == null)
		body->userObject = new PlanetariumUserObject(color);
	else
		static_cast<PlanetariumUserObject*>(body->userObject)->color = color;

	addCustomBody(body);
}


/** Adds (safely) a custom body with the given parameters. If no color is specified, a random color will be used. */
void Planetarium::addCustomBody(double mass, double diameter, const Vector2D& position, const Vector2D& velocity, const SDL_Color& color)
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
		gauss_random_between(avMass*0.9, avMass*1.1),
		gauss_random_between(avDiameter*0.9, avDiameter*1.1),
		Vector2D(random_between(minPosX, maxPosX), random_between(minPosY, maxPosY)),
		Vector2D(gauss_random_between(avSpeed*0.9, avSpeed*1.1)),
		SDL_util::getRandomColor()
	);
}

/** Adds a random body with random traits. If an area is specified, the resulting body will be positioned randomly within it.*/
void Planetarium::addRandomBody(const double area[4])
{
	double az = 1/viewportZoom;
	double diameter = (bodyCreationDiameterRatio * az) * BODY_CREATION_DIAMETER_FACTOR;
	double mass = (M_PI/6.0) * bodyCreationDensity * pow(diameter, 3);
	double speed = bodyCreationSpeed * az;

	addRandomBody(mass, diameter, speed, area);
}

/** Adds a random body with random traits. If an area is specified, the resulting body will be positioned randomly within it.*/
void Planetarium::addRandomOrbitingBody(const double area[4], const bool& clockwiseArg)
{
	bool clockwise = (&clockwiseArg == &DEFAULT_ADD_RANDOM_ORBITING_ORIENTATION? (rand()%2==0) : clockwiseArg);
	double az = 1/viewportZoom;
	double diameter = (bodyCreationDiameterRatio * az) * BODY_CREATION_DIAMETER_FACTOR;
	diameter = gauss_random_between(diameter*0.9, diameter*1.1);
	double mass = (M_PI/6.0) * bodyCreationDensity * pow(diameter, 3);
	mass = gauss_random_between(mass*0.9, mass*1.1);

	const double minPosX = area != null ? area[0] : DBL_MIN,
				 minPosY = area != null ? area[1] : DBL_MIN,
				 maxPosX = area != null ? area[0] + area[2] : DBL_MAX,
				 maxPosY = area != null ? area[1] + area[3] : DBL_MAX;

	Vector2D position(random_between(minPosX, maxPosX), random_between(minPosY, maxPosY));

	Vector2D centerOfMass, centerOfMassVelocity;
	double totalMass;

	getCurrentOrbitalReference(centerOfMass, centerOfMassVelocity, totalMass);

	Vector2D distanceToCenter = position.difference(centerOfMass);

	double speed = sqrt((2*physics->universe.gravity*totalMass)/distanceToCenter.length());
	speed = gauss_random_between(speed*0.7, speed*0.9);
	Vector2D velocity = distanceToCenter.unit().perpendicular().scale(speed);
	if(not clockwise) velocity.reflect();
	velocity.add(centerOfMassVelocity);

	addCustomBody(mass, diameter, position, velocity, SDL_util::getRandomColor());

	//source info:
	//http://physics.stackexchange.com/questions/227502/orbital-mechanics-will-a-satellite-crash?rq=1
}

void Planetarium::recolorAllBodies()
{
	synchronized(physicsAccessMutex)
	{
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
			static_cast<PlanetariumUserObject*>(body->userObject)->color = SDL_util::getRandomColor();
	}
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
	// Generate user object if not provided
	const_foreach(Body2D*, i, vector<Body2D*>, u.bodies)
		if(i->userObject == null)
			i->userObject = new PlanetariumUserObject(SDL_util::getRandomColor());

	vector<Body2D*> removedBodiesPointers;

	synchronized(physicsAccessMutex)
	{
		// write down all bodies
		foreach(Body2D*, body, vector<Body2D*>, this->physics->universe.bodies)
			removedBodiesPointers.push_back(body);

		//copy universe. user objects are the same as the copy. 'removedBodiesPointers' bodies are detatched after this.
		physics->setUniverse(u);
	}

	focusedBodies.clear();
	physics->referenceFrame.reset();

	foreach(Body2D*, body, vector<Body2D*>, removedBodiesPointers)
	{
		//notify listeners about the body deleted
		for(unsigned i = 0; i < listeners.size(); i++)
			listeners[i]->onBodyDeletion(body);

		orbitTracer.clearTrace(body);

		//delete the user objects of the old universe, as setUniverse() will delete the old universe, but not its bodies (and user objects)
		delete static_cast<PlanetariumUserObject*>(body->userObject);
		delete body; //trash it
	}

	stateManager->reset();
}

void Planetarium::setSolver(AbstractPhysics2DSolver* solver)
{
	synchronized(physicsAccessMutex)
	{
		physics->setSolver(solver);
	}
}

void Planetarium::undoLastChange()
{
	stateManager->rollback();
}

void Planetarium::rewindLastChange()
{
	stateManager->rollback(true);
}

void Planetarium::setUndoEnabled(bool choice)
{
	undoEnabled = choice;
	if(choice == false) stateManager->reset();
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
				double mass = (M_PI/6.0) * bodyCreationDensity * bodyCreationDiameter * bodyCreationDiameter * bodyCreationDiameter;
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
								if(remove_element(focusedBodies, body) == false)
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
							if(remove_element(focusedBodies, body) == false)
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

void Planetarium::OrbitTracer::record(Body2DClone& body)
{
	this->traces[body.original].push_back(body.clone.position-planetarium->physics->referenceFrame.position()); //queue push
	while(this->traces[body.original].size() > traceLength)
		this->traces[body.original].pop_front(); //queue pop
}

deque<Vector2D>& Planetarium::OrbitTracer::getTrace(Body2D* body)
{
	return this->traces[body];
}

void Planetarium::OrbitTracer::clearTrace(const Body2D* body)
{
	this->traces.erase(const_cast<Body2D*>(body));
}

void Planetarium::OrbitTracer::reset()
{
	this->traces.clear();
}

void Planetarium::OrbitTracer::drawTrace(Body2D* body)
{
	if(body->userObject == null) return; //if there isn't body data, there isn't body color. leave.
	SDL_Color& bodyColor = static_cast<PlanetariumUserObject*>(body->userObject)->color;

	deque<Vector2D>& trace = this->getTrace(body);

	drawTrace(trace, bodyColor);
}

void Planetarium::OrbitTracer::drawTrace(Body2DClone& body)
{
	if(body.clone.userObject == null) return; //if there isn't body data, there isn't body color. leave.
	SDL_Color& bodyColor = static_cast<PlanetariumUserObject*>(body.clone.userObject)->color;

	deque<Vector2D>& trace = this->getTrace(body.original); //get original body trace

	drawTrace(trace, bodyColor);
}

void Planetarium::OrbitTracer::drawTrace(deque<Vector2D>& trace, const SDL_Color& color)
{
	switch(style)
	{
		case DOTTED:			drawDotted(trace, color); break;
		default:case LINEAR:	drawLinear(trace, color); break;
		case SPLINE:			drawQuadricBezier(trace, color); break;
	}
}

void Planetarium::OrbitTracer::drawDotted(deque<Vector2D>& trace, const SDL_Color& bodyColor)
{
	foreach(Vector2D&, r, deque<Vector2D>, trace)
	{
		Vector2D pv = planetarium->getTransposedNoRef(r);
		pixelRGBA(planetarium->surf, round(pv.x), round(pv.y), bodyColor.r, bodyColor.g, bodyColor.b, 255);
	}
}

void Planetarium::OrbitTracer::drawLinear(deque<Vector2D>& trace, const SDL_Color& bodyColor)
{
	int (*const line_function) (SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (planetarium->tryAA? aalineRGBA : lineRGBA);

	Vector2D previousPosition = trace.front();
	foreach(Vector2D&, recordedPosition, deque<Vector2D>, trace)
	{
		if(recordedPosition != previousPosition) //avoid drawing segments of same points
		{
			Vector2D recPosTrans = planetarium->getTransposedNoRef(recordedPosition), prevPosTrans = planetarium->getTransposedNoRef(previousPosition);
			line_function(planetarium->surf, round(prevPosTrans.x), round(prevPosTrans.y), round(recPosTrans.x), round(recPosTrans.y), bodyColor.r, bodyColor.g, bodyColor.b, 255);
		}
		previousPosition = recordedPosition;
	}
}

// http://stackoverflow.com/questions/9658932/snappy-bezier-curves
// http://www.ferzkopp.net/Software/SDL_gfx-2.0/Docs/html/_s_d_l__gfx_primitives_8h.html#a7203e3a463da499b5b0cadf211d19ff3
void Planetarium::OrbitTracer::drawQuadricBezier(deque<Vector2D>& trace, const SDL_Color& bodyColor)
{
	Vector2D previousPosition = trace.front();
	Vector2D previousSupport;
//	if(trace.size() > 1) previousSupport = trace.front().times(3).subtract(*(trace.begin()+1)).scale(0.5); //kickstart aux
	if(trace.size() > 1) previousSupport = trace.front().times(2).subtract(*(trace.begin()+1)); //kickstart aux
	foreach(Vector2D&, recordedPosition, deque<Vector2D>, trace)
	{
		if(recordedPosition != previousPosition) //avoid drawing segments of same points
		{
			//fixme Fix quadratic bezier spline implementation
			Vector2D supportPoint = planetarium->getTransposedNoRef(previousSupport);
			Vector2D recPosTrans = planetarium->getTransposedNoRef(recordedPosition), prevPosTrans = planetarium->getTransposedNoRef(previousPosition);
			Sint16 pxs[] = {static_cast<Sint16>(prevPosTrans.x), static_cast<Sint16>(supportPoint.x), static_cast<Sint16>(recPosTrans.x)};
			Sint16 pys[] = {static_cast<Sint16>(prevPosTrans.y), static_cast<Sint16>(supportPoint.y), static_cast<Sint16>(recPosTrans.y)};
			bezierRGBA(planetarium->surf, pxs, pys, 3, 3, bodyColor.r, bodyColor.g, bodyColor.b, 255);
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
		if(running and (not legacyControl or currentIterationCount++ < iterationsPerDisplay))
		{
			synchronized(physicsAccessMutex)
			{
				this->physics->step();
			}
		}
		SDL_Delay(legacyControl? 0 : stepDelay);
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
	//constructs a PlanetariumUserObject for the merger body
	long double r_sum=0, g_sum=0, b_sum=0, mass_sum=0;
	foreach(Body2D*, body, vector<Body2D*>, collidingList)
	{
		const SDL_Color& bodyColor = static_cast<PlanetariumUserObject*> (body->userObject)->color;
		r_sum += bodyColor.r * body->mass;
		g_sum += bodyColor.g * body->mass;
		b_sum += bodyColor.b * body->mass;
		mass_sum += body->mass;
		orbitTracer.clearTrace(body);
	}
	if(mass_sum==0) mass_sum=1; //avoid division by zero if erroneous zero-mass or zero-sum-mass bodies were passed

	SDL_Color meanColor;
	meanColor.r = static_cast<Uint8>(r_sum/mass_sum);
	meanColor.g = static_cast<Uint8>(g_sum/mass_sum);
	meanColor.b = static_cast<Uint8>(b_sum/mass_sum);

	resultingMerger.userObject = new PlanetariumUserObject(meanColor);

	if(undoEnabled) //if undo is enabled, commit merge change instead of deleting user objects
	{
		vector<Body2D*> collidingListBackup;
		foreach(Body2D*, mergedBody, vector<Body2D*>, collidingList)
		{
			//backup merged bodies as they will be deleted by the physics thread
			Body2D* mergedBodyBackup = new Body2D(*mergedBody);
			collidingListBackup.push_back(mergedBodyBackup);

			//swap pointers to the merged bodies with pointers to their backups
			stateManager->fixReferences(mergedBody, mergedBodyBackup);
		}

		stateManager->commitMerge(collidingListBackup, &resultingMerger);
	}
	else //delete user objects of collided bodies (the physics code won't do it as it has no knowledge of this)
		purgeUserObjects(collidingList);

	//creates a collision event for listeners
	CollisionEvent* ev = new CollisionEvent();
	foreach(Body2D*, body, vector<Body2D*>, collidingList)
	{
		ev->collidingBodies.push_back(*body);
		ev->collidingBodies.back().userObject = null; //users cannot rely on user object availability
	}
	ev->resultingMerger = resultingMerger;

	//push user event to the event queue
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
			mass = 0;
			for(unsigned i = 0; i < bodies.size(); i++)
			{
				position.add(bodies[i].clone.position.times(bodies[i].clone.mass));
				velocity.add(bodies[i].clone.velocity.times(bodies[i].clone.mass));
				mass += bodies[i].clone.mass;
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

bool Planetarium::isPastRocheLimit(const Body2D& body, const Vector2D& primaryPosition, const double& primaryMass)
{
	const double distance = body.position.distance(primaryPosition);
	if(distance < 0.5*body.diameter)
		return false;

	const double rocheLimit = 0.5*body.diameter*pow(2.0*primaryMass/body.mass, 1.0/3.0);
	return distance < rocheLimit;
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

// ------------------------------- functions -----------------------------

void purgeUserObjects(std::vector<Body2D*>& bodies)
{
	foreach(Body2D*, oldBody, vector<Body2D*>, bodies)
		delete static_cast<PlanetariumUserObject*>(oldBody->userObject);
}

void purgeUserObjects(std::vector<Body2D>& bodies)
{
	foreach(Body2D&, oldBody, vector<Body2D>, bodies)
		delete static_cast<PlanetariumUserObject*>(oldBody.userObject);
}

void purgeUserObjects(vector<Planetarium::Body2DClone>& bodies, bool onOriginal)
{
	foreach(Planetarium::Body2DClone&, oldBody, vector<Planetarium::Body2DClone>, bodies)
		if(not onOriginal)
			delete static_cast<PlanetariumUserObject*>(oldBody.clone.userObject);
		else
			delete static_cast<PlanetariumUserObject*>(oldBody.original->userObject);
}
