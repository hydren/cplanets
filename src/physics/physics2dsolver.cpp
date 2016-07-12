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

/// A collection of AbstractPhysics2DSolver factories, ready to be used. Each factory can instantiate some subclass of AbstractPhysics2DSolver.
vector<const AbstractPhysics2DSolver::GenericFactory*> AbstractPhysics2DSolver::registeredFactories;

void AbstractPhysics2DSolver::computeAllBodiesAccelerations()
{
	foreach(Body2D*, b1, vector<Body2D*>, universe.bodies)
	{
		b1->acceleration *= 0; //clear the acceleration

		foreach(Body2D*, b2, vector<Body2D*>, universe.bodies)
		{
			if(b1 != b2)
			{
				double force = -universe.gravity*b1->mass*b2->mass / pow(b1->position.distance(b2->position), 2);
				b1->acceleration += (b1->position - b2->position).normalize() * (force/b1->mass);
			}
		}
	}
}

Vector2D AbstractPhysics2DSolver::calculateAccelerationDueToNeighborhood(Vector2D position, Body2D* body)
{
	Vector2D acc;
	foreach(Body2D*, neighbor, vector<Body2D*>, universe.bodies)
	{
		if(neighbor != body)
		{
			double force = -universe.gravity*body->mass*neighbor->mass / pow(position.distance(neighbor->position), 2);
			acc += (position - neighbor->position).normalize() * (force/body->mass);
		}
	}
	return acc;
}
