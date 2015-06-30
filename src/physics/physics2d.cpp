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

