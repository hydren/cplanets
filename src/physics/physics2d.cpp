/*
 * physics2d.cpp
 *
 *  Created on: 23/06/2015
 *      Author: Carlos Faruolo
 */

#include "physics2d.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

#include "futil/futil.hpp"
using futil::throw_exception;

using std::vector;

Physics2D::Physics2D()
: universe(), referenceFrame(),
  physics2DSolver(null),
  onCollision(null),
  collisions(), collisionListeners(null)
{}

Vector2D ReferenceFrame::getPosition() const
{
	Vector2D centerOfMass;
	double totalMass = 0;
	const_foreach(const Body2D*, body, vector<Body2D*>, bodies)
	{
		const Body2D& b = *body;
		centerOfMass.add(b.position.times(b.mass));
		totalMass += b.mass;
	}

	centerOfMass.scale(1.0/totalMass);

	return centerOfMass;
}

Vector2D ReferenceFrame::getVelocity() const
{
	Vector2D totalVelocity;
	double totalMass = 0;
	const_foreach(const Body2D*, body, vector<Body2D*>, bodies)
	{
		const Body2D& b = *body;
		totalVelocity.add(b.velocity.times(b.mass));
		totalMass += b.mass;
	}

	totalVelocity.scale(1.0/totalMass);

	std::cout << "refential velocity: " << totalVelocity.x << " " << totalVelocity.y << std::endl;
	std::cout << "total mass of referential: " << totalMass << std::endl;

	return totalVelocity;
}

void Physics2D::step()
{
	if(physics2DSolver == null)
		throw_exception("Can't do step(): No physics solver specified!");

	AbstractPhysics2DSolver& solver = *physics2DSolver;

	//update positions
	solver.step();

	//resolve collisions
	resolveCollisions();
}

void Physics2D::changeReferenceFrameTo(vector<Body2D*>& reference)
{
	//FIXME TODO implement Physics2D::changeReferenceFrameTo
	//compute center of mass of 'reference'
	//compute total momentum for 'reference'
	//subtract from all bodies the position and speed
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

void Physics2D::resolveCollisions()
{
	//detect collisions
	foreach(Body2D*, ap, vector<Body2D*>, universe.bodies) foreach(Body2D*, bp, vector<Body2D*>, universe.bodies)
	{
		Body2D& a = *ap; Body2D& b = *bp;

		if(a == b) goto continue1;

		if(a.position.distance(b.position) < a.diameter/2 + b.diameter/2)
		{
			bool bothAdded=false;
			foreach(vector<Body2D*>&, list1, vector< vector<Body2D*> >, collisions)
			{
				if(Collections::containsElement(list1, a) && Collections::containsElement(list1, b)) //probably a duplicate lookup
				{
					bothAdded = true;
					goto break1;
				}
				else if(Collections::containsElement(list1, a) && not Collections::containsElement(list1, b)) //append colliding b
				{
					list1.push_back(&a);
					bothAdded = true;
					goto break1;
				}
				else if(!Collections::containsElement(list1, a) && Collections::containsElement(list1, b)) //append colliding a
				{
					list1.push_back(&a);
					bothAdded = true;
					goto break1;
				}
			}
			break1:
			if(!bothAdded) //new colliding pair
			{
				vector<Body2D*> newlist1;
				newlist1.push_back(&a);
				newlist1.push_back(&b);
				collisions.push_back(newlist1);
			}
		}
		continue1:;
	}

	if(collisions.empty() == false)
		referenceFrame.bodies.clear();

	//resolve collisions
	foreach(vector<Body2D*>&, collisionList, vector< vector<Body2D*> >, collisions)
	{
		Body2D merger(0, 0, Vector2D(), Vector2D(), Vector2D());
		foreach(Body2D*, body1, vector<Body2D*>, collisionList)
		{
			Body2D& body = *body1; //to simplify formulas
			merger.position.x += body.position.x * body.mass;
			merger.position.y += body.position.y * body.mass;

			merger.velocity.x = (merger.velocity.x*merger.mass + body.velocity.x*body.mass)/(merger.mass + body.mass);
			merger.velocity.y = (merger.velocity.y*merger.mass + body.velocity.y*body.mass)/(merger.mass + body.mass);

			merger.diameter = sqrt(merger.diameter*merger.diameter + body.diameter*body.diameter);
			merger.mass += body.mass;

			//merger.color = (merger.color + body.color)/2;

			Collections::removeElement(universe.bodies, body1); //remove actual pointer
		}

		if(collisionList.size() == 0) goto continue2;

		merger.position.x /= merger.mass;
		merger.position.y /= merger.mass;

		universe.bodies.push_back(new Body2D(merger));

		//callback for body collision
		if(onCollision != null)
			onCollision(collisionList, *universe.bodies.back());

		//also notify if there are any listeners
		if(collisionListeners != null)
			foreach(CollisionListener*, listener, vector<CollisionListener*>, *(this->collisionListeners))
				listener->onCollision(collisionList, *(universe.bodies.back()));

		continue2:;
	}

	//cleanup
	foreach(vector<Body2D*>&, collisionList, vector< vector<Body2D*> >, collisions)
		foreach(Body2D*, trash, vector<Body2D*>, collisionList)
			delete trash; //fixme this deletion crashes a lot (double-freeing)
	collisions.clear();
}
