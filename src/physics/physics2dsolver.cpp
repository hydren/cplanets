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

void AbstractPhysics2DSolver::computeAccelerations()
{
	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
		getAccelerationOnPosition(body->position, body, &body->acceleration);
}

Vector2D AbstractPhysics2DSolver::getAccelerationOnPosition(const Vector2D& position, Body2D* body)
{
	Vector2D acc;
	getAccelerationOnPosition(position, body, &acc);
	return acc;
}

void AbstractPhysics2DSolver::getAccelerationOnPosition(const Vector2D& position, Body2D* body, Vector2D* accPtr)
{
	const double& mass = body->mass;
	Vector2D& acceleration = accPtr->scale(0); //clear the acceleration and reference it
	foreach(Body2D*, neighbor, vector<Body2D*>, universe.bodies)
	{
		if(neighbor != body)
		{
			double force = (-universe.gravity * mass * neighbor->mass) / pow(position.distance(neighbor->position), 2);
			acceleration += (position - neighbor->position).normalize() * (force/mass);
		}
	}
}
