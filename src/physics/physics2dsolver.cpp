/*
 * physics2dsolver.cpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: carlosfaruolo
 */

#include "physics2dsolver.hpp"

#include <cmath>

#include "futil/futil.hpp"

using std::vector;
using std::map;

/// A collection of AbstractPhysics2DSolver factories, ready to be used. Each factory can instantiate some subclass of AbstractPhysics2DSolver.
vector<const AbstractPhysics2DSolver::GenericFactory*> AbstractPhysics2DSolver::registeredFactories;

/** Computes and updates all bodies' accelerations based on their mutual gravitation forces, using their current positions. */
void AbstractPhysics2DSolver::computeAccelerations()
{
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		body->acceleration *= 0; // let acceleration be zero

		foreach(Body2D*, neighbor, vector<Body2D*>, universe.bodies)
		if(neighbor != body)
		{
			double forceMagnitude = (-universe.gravity * body->mass * neighbor->mass) / pow(body->position % neighbor->position, 2); // % is a distance operator
			body->acceleration += (body->position - neighbor->position).normalize() * (forceMagnitude/body->mass);
		}
	}
}

/** Computes the acceleration (resulting from mutual gravitation forces) of all bodies, at the specified positions (instead of the bodies' current position).
 *  The resulting accelerations are stored on 'resultingAccelerations'. */
void AbstractPhysics2DSolver::computeAccelerations(map<Body2D*, Vector2D>& acc, const map<Body2D*, Vector2D>& pos)
{
	foreach(Body2D*, b, vector<Body2D*>, universe.bodies)
	{
		foreach(Body2D*, b2, vector<Body2D*>, universe.bodies)
		{
			if(b != b2)
			{
				const Vector2D &b1pos = Collections::getValueAt(pos, b),
						       &b2pos = Collections::getValueAt(pos, b2);

				double forceScalar = (-universe.gravity * b->mass * b2->mass) / pow(b1pos.distance(b2pos), 2);
				acc[b] += (b1pos - b2pos).normalize() * (forceScalar/b->mass);
			}
		}
	}
}

void AbstractPhysics2DSolver::derive(map<Body2D*, Vector2D>& dvdt, map<Body2D*, Vector2D>& dydt, map<Body2D*, Vector2D>& vn, map<Body2D*, Vector2D>& yn)
{
	computeAccelerations(dvdt, yn);
	dydt = vn;
}
