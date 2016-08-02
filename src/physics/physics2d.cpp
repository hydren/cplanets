/*
 * physics2d.cpp
 *
 *  Created on: 23/06/2015
 *      Author: Carlos Faruolo
 */

#include "physics2d.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>

#include <cstdlib>
#include <cmath>

using std::vector;

//aliases
bool (*containsBody)(vector<Body2D*>&, const Body2D*) = Physics2D::containsBody;
bool (*removeBody)(vector<Body2D*>&, const Body2D*) = Physics2D::removeBody;


Physics2D::Physics2D()
: universe(),
  solver(NULL),
  referenceFrame(),
  onCollisionCallback(NULL),
  listeners()
{}

void Physics2D::step()
{
	if(solver == NULL)
		throw std::runtime_error("Can't do step(): No physics solver specified!");

	//update positions
	solver->step();

	//resolve collisions
	resolveCollisions();

	//move reference frame manually if point-like (and frame has velocity)
	if(referenceFrame.isPointLike() and not referenceFrame.velocity().isZero())
		referenceFrame.customPosition.add(referenceFrame.velocity().times(solver->timestep));
}

/** Exchanges the planetarium current physics solver with the given instance. */
void Physics2D::setSolver(AbstractPhysics2DSolver* newSolver)
{
	AbstractPhysics2DSolver* older = solver;
	solver = newSolver; //swap solver
	delete older;
}

/** Exchanges the planetarium universe with a copy of the given instance. */
void Physics2D::setUniverse(const Universe2D& universe)
{
	// todo should we copy older timeElapsed as well? would break Leapfrog solver, though.
	double timestep = solver->timestep;
	this->universe = universe; // copies the universe (through the assignment)
	setSolver(solver->factory->createSolver(this->universe)); // create a solver with same class
	solver->timestep = timestep;
}

Vector2D Physics2D::ReferenceFrame::position() const
{
	if(bodies.size() == 0)
		return customPosition;
	else if(bodies.size() == 1) //optimization for 1 body
		return bodies[0]->position;
	else
	{
		Vector2D centerOfMass;
		double totalMass = 0;
		for(unsigned i = 0; i < bodies.size(); i++)
		{
			const Body2D& b = *(bodies[i]);
			centerOfMass.add(b.position.times(b.mass));
			totalMass += b.mass;
		}

		if(totalMass != 0) // this is for safety: obviously a total mass of 0 is something wrong...
			centerOfMass.scale(1.0/totalMass);

		return centerOfMass;
	}
}

Vector2D Physics2D::ReferenceFrame::velocity() const
{
	if(bodies.size() == 0)
		return customVelocity;
	else if(bodies.size() == 1) //optimization for 1 body
		return bodies[0]->velocity;
	else
	{
		Vector2D totalVelocity;
		double totalMass = 0;
		for(unsigned i = 0; i < bodies.size(); i++)
		{
			const Body2D& b = *(bodies[i]);
			totalVelocity.add(b.velocity.times(b.mass));
			totalMass += b.mass;
		}

		if(totalMass != 0) // this is for safety: obviously a total mass of 0 is something wrong...
			totalVelocity.scale(1.0/totalMass);

		return totalVelocity;
	}
}

bool Physics2D::ReferenceFrame::isPointLike() const
{
	return this->bodies.empty();
}

void Physics2D::ReferenceFrame::reset()
{
	this->bodies.clear();
	this->customPosition = Vector2D();
	this->customVelocity = Vector2D();
}

void Physics2D::ReferenceFrame::set(Vector2D position, Vector2D velocity)
{
	this->reset();
	this->customPosition = position;
	this->customVelocity = velocity;
}

void Physics2D::ReferenceFrame::set(const Body2D** reference, unsigned n)
{
	this->reset();
	for(unsigned i = 0; i < n; i++)
		this->bodies.push_back(reference[i]);
}

void Physics2D::ReferenceFrame::set(const std::vector<Body2D*>& reference)
{
	this->reset();
	this->bodies.assign(reference.begin(), reference.end());
}

vector<Body2D*>* collisionsOf(Body2D* body, vector< vector<Body2D*> >& collisions)
{
	for(unsigned i = 0; i < collisions.size(); i++)
	{
		if(containsBody(collisions[i], body))
			return &(collisions[i]);
	}

	return NULL;
}

void Physics2D::resolveCollisions()
{
	vector<Body2D*>& b = universe.bodies;
	vector< vector<Body2D*> > collisions;

	// detect collisions
	for(unsigned i = 0; i < universe.bodies.size(); i++)
	for(unsigned j = 0; j < universe.bodies.size(); j++)
	{
		if(b[i] != b[j]) // only make sense to deal with different bodies
		{
			if(b[i]->position.distance(b[j]->position) < b[i]->diameter/2.0 + b[j]->diameter/2.0)
			{
				vector<Body2D*>* bi_col_list = collisionsOf(b[i], collisions), *bj_col_list = collisionsOf(b[j], collisions);
				const bool bi_alreadyAdded = (bi_col_list != NULL);
				const bool bj_alreadyAdded = (bj_col_list != NULL);

				if(bi_alreadyAdded and bj_alreadyAdded)
				{
					if(bi_col_list != bj_col_list) // if they're both added but on different lists, merge lists
					{
						bi_col_list->insert(bi_col_list->end(), bj_col_list->begin(), bj_col_list->end());
						collisions.erase(std::find(collisions.begin(), collisions.end(), *bj_col_list));
					}
				}
				else if(bi_alreadyAdded and not bj_alreadyAdded)
				{
					bi_col_list->push_back(b[j]);
				}
				else if(not bi_alreadyAdded and bj_alreadyAdded)
				{
					bj_col_list->push_back(b[i]);
				}
				else // both not added
				{
					collisions.push_back(vector<Body2D*>()); //push new list
					vector<Body2D*>& newlist = collisions.back(); //grab the list to edit
					newlist.push_back(b[i]);
					newlist.push_back(b[j]);
				}
			}
		}
	}

	//if there was collisions, reset
	if(not collisions.empty())
		this->referenceFrame.reset();

	//resolve collisions
	for(unsigned k = 0; k < collisions.size(); k++)
	{
		Body2D merger(0, 0, Vector2D(), Vector2D(), Vector2D());
		for(unsigned n = 0; n < collisions[k].size(); n++)
		{
			Body2D& body = *(collisions[k][n]); //to simplify formulas
			merger.position.x += body.position.x * body.mass;
			merger.position.y += body.position.y * body.mass;

			merger.velocity.x = (merger.velocity.x*merger.mass + body.velocity.x*body.mass)/(merger.mass + body.mass);
			merger.velocity.y = (merger.velocity.y*merger.mass + body.velocity.y*body.mass)/(merger.mass + body.mass);

			merger.diameter = sqrt(merger.diameter*merger.diameter + body.diameter*body.diameter);
			merger.mass += body.mass;

			removeBody(universe.bodies, collisions[k][n]); //remove actual pointer
		}

		merger.position.x /= merger.mass;
		merger.position.y /= merger.mass;

		universe.bodies.push_back(new Body2D(merger)); //creates a copy and stores on universe.bodies

		this->notifyAll(collisions[k], *universe.bodies.back());
	}

	//cleanup
	for(unsigned k = 0; k < collisions.size(); k++)
	for(unsigned n = 0; n < collisions[k].size(); n++)
		delete collisions[k][n]; //delete previously existing bodies individually

	collisions.clear(); //removes all collisions lists
}

void Physics2D::notifyAll(vector<Body2D*>& collidingList, Body2D& resultingMerger)
{
	if(onCollisionCallback != NULL) onCollisionCallback(collidingList, resultingMerger);
	for(unsigned i = 0; i < listeners.size(); i++) listeners[i]->onCollision(collidingList, resultingMerger);
}

bool Physics2D::containsBody(vector<Body2D*>& collection, const Body2D* body)
{
	vector<Body2D*>::iterator it = std::find(collection.begin(), collection.end(), body);
	return it != collection.end();
}

bool Physics2D::removeBody(vector<Body2D*>& collection, const Body2D* element)
{
	vector<Body2D*>::iterator it = std::find(collection.begin(), collection.end(), element);
	if(it == collection.end()) return false;
	collection.erase(it);
	return true;
}
