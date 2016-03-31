/*
 * physics2dsolver.cpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: carlosfaruolo
 */

#include "physics2dsolver.hpp"

#include <vector>
#include <cmath>

#include "futil/futil.hpp"

using std::vector;

void AbstractPhysics2DSolver::computeAllBodiesAccelerations()
{
	foreach(Body2D*, b1p, vector<Body2D*>, universe.bodies)
	{
		Body2D& b1 = *b1p;
		b1.acceleration.scale(0);

		foreach(Body2D*, b2p, vector<Body2D*>, universe.bodies)
		{
			Body2D& b2 = *b2p;
			if(b1 != b2)
			{
				double force = -universe.gravity*b1.mass*b2.mass/pow(b1.position.distance(b2.position), 2);
				b1.acceleration.add(b1.position.difference(b2.position).normalize().scale(force/b1.mass));
			}
		}
	}
}

Vector2D AbstractPhysics2DSolver::calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body)
{
	Vector2D acc;
	foreach(Body2D*, bodyPtr, vector<Body2D*>, universe.bodies)
	{
		Body2D& body = *bodyPtr;
		if(body != body)
		{
			double force = -universe.gravity*body.mass*body.mass/pow(position.distance(body.position), 2);
			acc.add(position.difference(body.position).normalize().scale(force/body.mass));
		}
	}
	return acc;
}


