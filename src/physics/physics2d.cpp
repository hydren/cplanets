/*
 * physics2d.cpp
 *
 *  Created on: 23/06/2015
 *      Author: Visagio
 */

#include "physics/physics2d.hpp"
#include <iostream>

Vector2D ReferenceFrame::getPosition() const
{
	Vector2D centerOfMass;
	double totalMass = 0;
	foreach(Body2D&, b, set<Body2D>, bodies)
	{
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
	foreach(Body2D&, b, set<Body2D>, bodies)
	{
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

void Physics2D::changeReferenceFrameTo(set<Body2D*>& reference)
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
	foreach(Body2D&, a, set<Body2D*>, universe.bodies)	foreach(Body2D&, b, set<Body2D*>, universe.bodies)
	{
		if(a == b) continue;

		if(a.position.distance(b.position) < a.diameter/2 + b.diameter/2)
		{
			bool bothAdded=false;
			foreach(set<Body2D*>&, set, set< set<Body2D*> >, collisions)
			{
				if(Collections::containsElement(set, a) && Collections::containsElement(set, b)) //probably a duplicate lookup
				{
					bothAdded = true;
					break;
				}
				else if(Collections::containsElement(set, a) && not Collections::containsElement(set, b)) //append colliding b
				{
					set.insert(&a);
					bothAdded = true;
					break;
				}
				else if(!Collections::containsElement(set, a) && Collections::containsElement(set, b)) //append colliding a
				{
					set.insert(&a);
					bothAdded = true;
					break;
				}
			}
			if(!bothAdded) //new colliding pair
			{
				set<Body2D*> newSet;
				newSet.insert(&a);
				newSet.insert(&b);
				collisions.insert(newSet);
			}
		}
	}

	if(collisions.empty() == false)
		referenceFrame.bodies.clear();

	//resolve collisions
	foreach(set<Body2D*>&, collisionSet, set< set<Body2D*> >, collisions)
	{
		Body2D merger(0, 0, new Vector2D(), new Vector2D(), new Vector2D());
		foreach(Body2D&, body, set<Body2D*>, collisionSet)
		{
			merger.position.x += body.position.x;
			merger.position.y += body.position.y;

			merger.velocity.x = (merger.velocity.x*merger.mass + body.velocity.x*body.mass)/(merger.mass + body.mass);
			merger.velocity.y = (merger.velocity.y*merger.mass + body.velocity.y*body.mass)/(merger.mass + body.mass);

			merger.diameter = sqrt(merger.diameter*merger.diameter + body.diameter*body.diameter);
			merger.mass += body.mass;

			merger.color = (merger.color + body.color)/2;

			universe.bodies.erase(body);
		}

		if(collisionSet.size() == 0) continue;

		merger.position.x /= collisionSet.size();
		merger.position.y /= collisionSet.size();

		universe.bodies.insert(merger);

		//notify listeners about the collision
		foreach(BodyCollisionListener*, listener, set<BodyCollisionListener*>, registeredBodyCollisionListeners)
			listener->onBodyCollision(collisionSet, merger);
	}
}
