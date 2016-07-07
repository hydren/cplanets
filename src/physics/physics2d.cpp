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

#include <cmath>

#include "futil/futil.hpp"

using std::vector;

Physics2D::Physics2D()
: universe(),
  physics2DSolver(null),
  referenceFrame(),
  onCollision(null),
  collisionListeners(null)
{}

void Physics2D::step()
{
	if(physics2DSolver == null)
		throw std::runtime_error("Can't do step(): No physics solver specified!");

	AbstractPhysics2DSolver& solver = *physics2DSolver;

	//update positions
	solver.step();

	//resolve collisions
	resolveCollisions();

	//move reference frame manually if point-like (and frame has velocity)
	if(referenceFrame.isPointLike() and not referenceFrame.velocity().isZero())
		referenceFrame.customPosition.add(referenceFrame.velocity().times(solver.timestep));
}

Vector2D Physics2D::ReferenceFrame::position() const
{
	if(bodies.size() == 0)
		return customPosition;
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
	this->bodies.clear();
	this->customPosition = position;
	this->customVelocity = velocity;
}

void Physics2D::ReferenceFrame::set(const Body2D** reference, unsigned n)
{
	this->bodies.clear();
	for(unsigned i = 0; i < n; i++)
		this->bodies.push_back(reference[i]);
}

void Physics2D::ReferenceFrame::set(const std::vector<Body2D*>& reference)
{
	this->bodies.clear();
	this->bodies.assign(reference.begin(), reference.end());
}

void Physics2D::addCollisionListener(CollisionListener* listener)
{
	if(listener == null) return;
	if(this->collisionListeners == null) //instantiate on demand
		this->collisionListeners = new vector<CollisionListener*>();

	this->collisionListeners->push_back(listener);
}

void Physics2D::removeCollisionListener(CollisionListener* listener)
{
	if(listener == null) return;
	if(this->collisionListeners != null)
	{
		int index = Collections::indexOf(*(this->collisionListeners), listener);
		if(index >= 0) //found it
			this->collisionListeners->erase(this->collisionListeners->begin() + index);
	}
}



vector<Body2D*>* collisionsOf(Body2D* body, vector< vector<Body2D*> >& collisions)
{
	foreach(vector<Body2D*>&, list1, vector< vector<Body2D*> >, collisions)
		if(Collections::containsElement(list1, body))
				return &list1;

	return null;
}

void Physics2D::resolveCollisions()
{
	vector< vector<Body2D*> > collisions;

	// detect collisions
	foreach(Body2D*, b1, vector<Body2D*>, universe.bodies) foreach(Body2D*, b2, vector<Body2D*>, universe.bodies)
	{
		if(b1 != b2) // only make sense to deal with different bodies
		{
			if(b1->position.distance(b2->position) < b1->diameter/2.0 + b2->diameter/2.0)
			{
				vector<Body2D*>* b1_col_list = collisionsOf(b1, collisions), *b2_col_list = collisionsOf(b2, collisions);
				const bool b1_alreadyAdded = b1_col_list != null;
				const bool b2_alreadyAdded = b2_col_list != null;

				if(b1_alreadyAdded && b2_alreadyAdded)
				{
					if(b1_col_list != b2_col_list) // if they're both added but on different lists, merge lists
					{
						b1_col_list->insert(b1_col_list->end(), b2_col_list->begin(), b2_col_list->end());
						collisions.erase(std::find(collisions.begin(), collisions.end(), *b2_col_list));
					}
				}
				else if(b1_alreadyAdded && not b2_alreadyAdded)
				{
					b1_col_list->push_back(b2);
				}
				else if(not b1_alreadyAdded && b2_alreadyAdded)
				{
					b2_col_list->push_back(b1);
				}
				else // both not added
				{
					collisions.push_back(vector<Body2D*>()); //push new list
					vector<Body2D*>& newlist = collisions.back(); //grab the list to edit
					newlist.push_back(b1);
					newlist.push_back(b2);
				}
			}
		}
	}

	if(not collisions.empty())
		this->referenceFrame.position();

	//resolve collisions
	foreach(vector<Body2D*>&, collisionList, vector< vector<Body2D*> >, collisions)
	{
		Body2D merger(0, 0, Vector2D(), Vector2D(), Vector2D());
		foreach(Body2D*, bodyPtr, vector<Body2D*>, collisionList)
		{
			Body2D& body = *bodyPtr; //to simplify formulas
			merger.position.x += body.position.x * body.mass;
			merger.position.y += body.position.y * body.mass;

			merger.velocity.x = (merger.velocity.x*merger.mass + body.velocity.x*body.mass)/(merger.mass + body.mass);
			merger.velocity.y = (merger.velocity.y*merger.mass + body.velocity.y*body.mass)/(merger.mass + body.mass);

			merger.diameter = sqrt(merger.diameter*merger.diameter + body.diameter*body.diameter);
			merger.mass += body.mass;

			Collections::removeElement(universe.bodies, bodyPtr); //remove actual pointer
		}

		merger.position.x /= merger.mass;
		merger.position.y /= merger.mass;

		universe.bodies.push_back(new Body2D(merger)); //creates a copy and stores on universe.bodies

		//callback for body collision
		if(onCollision != null)
			onCollision(collisionList, *universe.bodies.back());

		//also notify if there are any listeners
		if(collisionListeners != null)
			foreach(CollisionListener*, listener, vector<CollisionListener*>, *(this->collisionListeners))
				listener->onCollision(collisionList, *universe.bodies.back());
	}

	//cleanup
	foreach(vector<Body2D*>&, collisionList, vector< vector<Body2D*> >, collisions)
		foreach(Body2D*, trash, vector<Body2D*>, collisionList)
			delete trash; //delete previously existing bodies individually
	collisions.clear(); //removes all collisions lists
}
