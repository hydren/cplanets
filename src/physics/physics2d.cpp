/*
 * physics2d.cpp
 *
 *  Created on: 23/06/2015
 *      Author: Carlos Faruolo
 */

#include "physics/physics2d.hpp"
#include <iostream>
#include <cmath>

Vector2D ReferenceFrame::getPosition() const
{
	Vector2D centerOfMass;
	double totalMass = 0;
	const_foreach(const Body2D*, body, list<Body2D*>, bodies)
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
	const_foreach(const Body2D*, body, list<Body2D*>, bodies)
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
	AbstractPhysics2DSolver& solver = *physics2DSolver;

	//update positions
	solver.step();

	//resolve collisions
	resolveCollisions();
}

void Physics2D::changeReferenceFrameTo(list<Body2D*>& reference)
{
	//FIXME TODO implement Physics2D::changeReferenceFrameTo
	//compute center of mass of 'reference'
	//compute total momentum for 'reference'
	//subtract from all bodies the position and speed
}

void Physics2D::resolveCollisions()
{
	//detect collisions
	collisions.clear();
	foreach(Body2D&, a, list<Body2D>, universe.bodies)	foreach(Body2D&, b, list<Body2D>, universe.bodies)
	{
		if(a == b) continue;

		if(a.position.distance(b.position) < a.diameter/2 + b.diameter/2)
		{
			bool bothAdded=false;
			foreach(list<Body2D*>&, list1, list< list<Body2D*> >, collisions)
			{
				if(Collections::containsElement(list1, a) && Collections::containsElement(list1, b)) //probably a duplicate lookup
				{
					bothAdded = true;
					break;
				}
				else if(Collections::containsElement(list1, a) && not Collections::containsElement(list1, b)) //append colliding b
				{
					list1.push_back(&a);
					bothAdded = true;
					break;
				}
				else if(!Collections::containsElement(list1, a) && Collections::containsElement(list1, b)) //append colliding a
				{
					list1.push_back(&a);
					bothAdded = true;
					break;
				}
			}
			if(!bothAdded) //new colliding pair
			{
				list<Body2D*> newlist1;
				newlist1.push_back(&a);
				newlist1.push_back(&b);
				collisions.push_back(newlist1);
			}
		}
	}

	if(collisions.empty() == false)
		referenceFrame.bodies.clear();

	//resolve collisions
	foreach(list<Body2D*>&, collisionList, list< list<Body2D*> >, collisions)
	{
		Body2D merger(0, 0, Vector2D(), Vector2D(), Vector2D());
		foreach(Body2D*, body1, list<Body2D*>, collisionList)
		{
			Body2D& body = *body1;
			merger.position.x += body.position.x;
			merger.position.y += body.position.y;

			merger.velocity.x = (merger.velocity.x*merger.mass + body.velocity.x*body.mass)/(merger.mass + body.mass);
			merger.velocity.y = (merger.velocity.y*merger.mass + body.velocity.y*body.mass)/(merger.mass + body.mass);

			merger.diameter = sqrt(merger.diameter*merger.diameter + body.diameter*body.diameter);
			merger.mass += body.mass;

			merger.color = (merger.color + body.color)/2;

			universe.bodies.remove(body);
		}

		if(collisionList.size() == 0) continue;

		merger.position.x /= collisionList.size();
		merger.position.y /= collisionList.size();

		universe.bodies.push_back(merger);

		//notify listeners about the collision
		foreach(BodyCollisionListener*, listener, list<BodyCollisionListener*>, registeredBodyCollisionListeners)
			listener->onBodyCollision(collisionList, merger);
	}
}
