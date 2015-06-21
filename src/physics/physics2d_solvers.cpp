/*
 * physics2d_solvers.cpp
 *
 *  Created on: 21/06/2015
 *      Author: Visagio
 */

#include "physics/physics2d_solvers.hpp"

#include <set>
using std::set;

#include <cmath>

void AbstractPhysics2DSolver::computeAllBodiesAccelerations()
{
	foreach(Body2D, b1, set<Body2D>, universe->bodies)
	{
//		if(b1.acceleration == NULL)
//			b1.acceleration = Vector2D();
//		else
			b1.acceleration.scale(0);

		foreach(Body2D, b2, set<Body2D>, universe->bodies)
		{
			if(b1 != b2)
			{
				double force = -universe->gravity*b1.mass*b2.mass/pow(b1.position.distance(b2.position), 2);
				b1.acceleration.add(b1.position.difference(b2.position).normalize().scale(force/b1.mass));
			}
		}
	}
}

Vector2D AbstractPhysics2DSolver::calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body)
{
	Vector2D acc;
	foreach(Body2D, b2, set<Body2D>, universe->bodies)
	{
		if(b2 != body)
		{
			double force = -universe->gravity*body.mass*b2.mass/pow(position.distance(b2.position), 2);
			acc.add(position.difference(b2.position).normalize().scale(force/body.mass));
		}
	}
	return acc;
}